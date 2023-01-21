#include <render/spheretracer.h>

#include <render/camera.h>
#include <render/color.h>
#include <render/film.h>
#include <render/sampler.h>

float SignedDistanceFunction::eval(Point3D p) const
{
    /**
     * SDF implementations based on code by Inigo Quilez
     * https://iquilezles.org/articles/distfunctions/
     */
    switch (function) {
    case Type::Sphere:
        // surface at distance 1 from the origin
        return p.norm() - 1.0f;
    case Type::Torus: {
        const float ra = 0.7f;
        const float rb = 0.3f;

        // circle in x,z plane at radius ra
        const Vector2D q{Vector2D{p.x, p.z}.norm() - ra, p.y};
        // surface at distance rb from that circle
        return q.norm() - rb;
    }
    case Type::Octahedron: {
        p = abs(p);
        const float m = p.x + p.y + p.z - 1.0f;
        Vector3D q;
        if (3.0f * p.x < m)
            q = {p.x, p.y, p.z};
        else if (3.0f * p.y < m)
            q = {p.y, p.z, p.x};
        else if (3.0f * p.z < m)
            q = {p.z, p.x, p.y};
        else
            return m * std::sqrt(1.0f / 3.0f);

        const float k = std::clamp(0.5f * (q.z - q.y + 1.0f), 0.0f, 1.0f);
        return Vector3D{q.x, q.y - 1.0f + k, q.z - k}.norm();
    }
    }
    return 0.0f;
}

bool SphereTracer::setParams(const SphereTracerParameters params,
                             const CameraParameters& cameraParams)
{
    SphereTracerParameters temp{params};
    temp.maxSPP = this->params.maxSPP;
    const bool needRestart = this->params != temp || this->cameraParams != cameraParams
                          || params.maxSPP < sppRendered
                          || (params.maxSPP > sppRendered && sppRendered == this->params.maxSPP);

    if (needRestart) {
        stop();

        this->cameraParams = cameraParams;
        camera = {cameraParams};
        if (film.getResolution() != cameraParams.resolution)
            film = {cameraParams.resolution};
        else
            film.clearWeights();
    }

    this->params = params;

    return needRestart;
}

void SphereTracer::start()
{
    stop();

    keep_rendering = true;
    renderThread = std::thread(&SphereTracer::render, this);
}

void SphereTracer::stop()
{
    keep_rendering = false;
    if (renderThread.joinable())
        renderThread.join();
}

void SphereTracer::render()
{
    const SignedDistanceFunction sdf{params.sdf};

    const Resolution resolution = film.getResolution();
    const Point2D invResolution = Point2D{resolution}.inverse();

    auto blockPos = [](uint32_t blockSize, uint32_t blockSampleIndex) -> Pixel {
        // 0, 1, 0, 1
        auto hatch0 = [](uint32_t x) -> uint32_t { return x & 1; };
        // 0, 1, 1, 0
        auto hatch1 = [](uint32_t x) -> uint32_t { return (x ^ (x >> 1)) & 1; };

        uint32_t lowX = 0;
        uint32_t lowY = 0;
        for (uint32_t i = 0; (blockSize >> (i + 1)); ++i) {
            if (i & 1) {
                lowX += hatch0(blockSampleIndex >> (2 * i)) * (blockSize >> (i + 1));
                lowY += hatch1(blockSampleIndex >> (2 * i)) * (blockSize >> (i + 1));
            }
            else {
                lowX += hatch1(blockSampleIndex >> (2 * i)) * (blockSize >> (i + 1));
                lowY += hatch0(blockSampleIndex >> (2 * i)) * (blockSize >> (i + 1));
            }
        }

        return {lowX, lowY};
    };

    uint32_t blockResDivider = 1;
    sppRendered = 0;
    partialSPPRendered = 0.0f;

    while (keep_rendering && sppRendered < params.maxSPP) {
        const Point2D sub_pixel = Sampler::halton(sppRendered);

        const uint32_t blockSize = 32;
        const Resolution numBlocks{(resolution.x + blockSize - 1) / blockSize,
                                   (resolution.y + blockSize - 1) / blockSize};
        const float invNumBlock = 1.0f / static_cast<float>(numBlocks.x * numBlocks.y);

        uint32_t index = 0;

        for (--blockResDivider; blockSize >> blockResDivider; ++blockResDivider) {
            const uint32_t pixelSize = blockSize >> blockResDivider;
            const uint32_t nextResIndex =
                numBlocks.x * numBlocks.y * (1U << (2U * blockResDivider));

#if defined(_WIN32)
            using OMPIndex = int32_t;
#else
            using OMPIndex = uint32_t;
#endif

#pragma omp parallel for schedule(dynamic)
            for (OMPIndex i = index; i < nextResIndex; ++i) {
                if (!keep_rendering)
                    continue;

                if (i % (blockSize * blockSize) == 0) {
                    new_sample_available.store(true, std::memory_order_relaxed);
                    partialSPPRendered += invNumBlock;
                }

                const uint32_t block = i % (numBlocks.x * numBlocks.y);
                const uint32_t inBlockSample = i / (numBlocks.x * numBlocks.y);
                const Pixel blockPixel = blockPos(blockSize, inBlockSample);
                const Pixel pixel{block % numBlocks.x * blockSize + blockPixel.x,
                                  block / numBlocks.x * blockSize + blockPixel.y};

                if (pixel.x >= resolution.x || pixel.y >= resolution.y)
                    continue;

                const Point2D normalizedScreenCoords =
                    ((Point2D{pixel} + sub_pixel) * invResolution - 0.5f) * 2.0f;
                const Ray ray = camera.generateRay(normalizedScreenCoords);

                AABB bounds{-1.0f, 1.0f};
                Color color{};

                // first check if the ray hits the cube between (-1,-1,-1) and (1,1,1)
                if (Intersection::intersect(bounds, ray)) {
                    const auto [pos, numSteps] = sdf.sphereTrace(ray);

                    if ((pos - ray.origin).norm() <= ray.tMax) {
                        switch (params.mode) {
                        case SphereTracerParameters::RenderMode::Depth:
                            bounds.extend(ray.origin);
                            color =
                                Color{(pos - ray.origin).norm() / bounds.extents().maxComponent()};
                            break;
                        case SphereTracerParameters::RenderMode::Position:
                            color = Color{(pos - bounds.min) / bounds.extents()};
                            break;
                        case SphereTracerParameters::RenderMode::Normal:
                            color = Color{sdf.computeNormal(pos) * 0.5f + 0.5f};
                            break;
                        case SphereTracerParameters::RenderMode::NumSteps:
                            color = Color{static_cast<float>(numSteps)};
                            break;
                        }
                    }
                }

                if (sppRendered)
                    film.addPixelColor(pixel, color);
                else
                    film.addPixelColorUnweighted(pixel, color, static_cast<int32_t>(pixelSize));
            }
            index = nextResIndex;
            new_sample_available.store(true, std::memory_order_relaxed);
        }
        ++sppRendered;
        partialSPPRendered = 0.0f;
    }
}
