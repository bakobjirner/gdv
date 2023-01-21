#ifndef SPHERETRACER_H
#define SPHERETRACER_H

#include "camera.h"
#include "film.h"
#include "intersection.h"
#include "ray.h"
#include "scene.h"

#include <atomic>
#include <thread>
#include <utility>

struct SignedDistanceFunction {
    enum class Type { Sphere, Torus, Octahedron } function{Type::Torus};

    bool operator==(const SignedDistanceFunction& other) const = default;

    float eval(Point3D p) const;

    /**
     * @brief sphereTrace
     * @param ray
     * @return intersection position or position outside the bounds and number of iterations
     */
    std::pair<Point3D, uint32_t> sphereTrace(Ray ray) const;
    /**
     * @brief computeNormal
     * @param pos
     * @return surface normal at the given position computed with central differences
     */
    Normal3D computeNormal(Point3D pos) const;
};

struct SphereTracerParameters {
    enum class RenderMode { Depth, Position, Normal, NumSteps };

    RenderMode mode{RenderMode::Depth};
    SignedDistanceFunction sdf{};
    /// maximum number of samples per pixel to render (don't burn the CPU too much)
    uint16_t maxSPP{32};

    bool operator==(const SphereTracerParameters& other) const = default;
};

class SphereTracer final {
public:
    SphereTracer() = default;
    ~SphereTracer() { stop(); }

    bool setParams(const SphereTracerParameters params, const CameraParameters& cameraParams);

    void start();
    void stop();

    bool imageHasChanged() const { return new_sample_available.exchange(false); }
    float getSPPRendererd() const { return sppRendered + partialSPPRendered; }

    const Camera& getCamera() const { return camera; }
    const Film& getFilm() const { return film; }

private:
    std::thread renderThread{};
    Camera camera{};
    Film film{};
    CameraParameters cameraParams;
    SphereTracerParameters params;
    bool keep_rendering{false};
    mutable std::atomic_bool new_sample_available{false};
    uint16_t sppRendered{};
    float partialSPPRendered{};

private:
    void render();
};

#endif // !SPHERETRACER_H
