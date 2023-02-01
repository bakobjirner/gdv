#include <render/raytracer.h>

#include <render/camera.h>
#include <render/color.h>
#include <render/film.h>
#include <render/intersection.h>
#include <render/sampler.h>
#include <render/scene.h>

Color RayTracer::volumetricPathIntegrator(const Ray& cameraRay) const
{
    Ray ray{cameraRay};

    ShadingIntersection its{scene, ray};
    if (!its)
        return {};

    // assume we start in vacuum
    Material::VolumeMedium currentMedium{};

    auto toWorld = [&its](const auto& v) { return its.shadingFrame.toWorld(v); };
    auto toLocal = [&its](const auto& v) { return its.shadingFrame.toLocal(v); };

    /// the final radiance arriving at the camera
    Color result{0.0f};
    /// attenuation of emitted radiance due to previous intersections (BRDF*cosine/PDF)
    Color throughput{1.0f};

    /// sample area lights or just compute their contribution via brute-force path tracing
    const bool sampleAreaLights = true;
    /// the last surface was rough and allowed us to sample direct illumination
    /// in this case we may not include illumination on intersected emitters again
    bool wasRough = false;

    for (uint32_t depth = 1; its && depth <= params.maxDepth; ++depth) {

        /// the material at the current intersection
        const Material& material = scene.getInstances().at(its.instanceIndex).material;
        /// the direction towards the camera
        const Vector3D omegaO = toLocal(-ray.direction);

        // backfaces are black (except for transmissive materials)
        if (omegaO.z < 0.0f && !material.isTransmissive())
            break;

        // transmittance in medium
        if (std::holds_alternative<Material::Absorption>(currentMedium)) {
            Material::Absorption absorption = std::get<Material::Absorption>(currentMedium);
            throughput *= absorption.transmittance(its.distance);

            if (throughput.isBlack())
                break;
        }

        // potentially add emitted radiance from an emitter
        // (but only if we did not sample direct light at the previous intersection)
        if ((!sampleAreaLights || !wasRough) && material.isEmitter())
            result += throughput * material.emittedRadiance;

        if (material.isRough()) {
            wasRough = true;

            // on diffuse surfaces, we can sample direct light
            if (depth < params.maxDepth) {
                for (const auto& light : scene.getLights()) {
                    if (!sampleAreaLights && !light.isPoint())
                        continue;

                    const auto [Li, pos] = light.sampleLi(its.point);
                    Ray shadowRay = Ray::shadowRay(its.point, pos);
                    const Vector3D omegaI = its.shadingFrame.toLocal(shadowRay.direction);
                    if (omegaI.z <= 0.0f)
                        continue;
                    const Color contribution =
                        throughput * Li * material.eval(omegaO, omegaI) * omegaI.z;
                    if (contribution.isBlack())
                        continue;

                    ShadingIntersection shadowIts{scene, shadowRay};
                    Color transmittance{1.0f};
                    Material::VolumeMedium shadowMedium{currentMedium};
                    while (shadowIts && !transmittance.isBlack()) {
                        const Material& shadowMaterial =
                            scene.getInstances().at(shadowIts.instanceIndex).material;
                        if (!shadowMaterial.isNull()) {
                            transmittance = {0.0f};
                            break;
                        }
                        if (std::holds_alternative<Material::Absorption>(shadowMedium))
                            transmittance *= std::get<Material::Absorption>(shadowMedium)
                                                 .transmittance(shadowIts.distance);
                        if (shadowMaterial.containsMedium()) {
                            if (dot(shadowIts.shadingFrame.n, shadowRay.direction) >= 0.0f)
                                shadowMedium = {};
                            else
                                shadowMedium = shadowMaterial.medium;
                        }
                        shadowRay = {shadowIts.point, shadowRay.direction, epsilon,
                                     shadowRay.tMax - shadowIts.distance};
                        shadowIts = {scene, shadowRay};
                    }
                    // transmittance if there is no intersection
                    if (std::holds_alternative<Material::Absorption>(shadowMedium))
                        transmittance *= std::get<Material::Absorption>(shadowMedium)
                                             .transmittance(shadowRay.tMax);
                    if (transmittance.isBlack())
                        continue;

                    result += transmittance * contribution;
                }
            }

            const Vector3D omegaI = Sampler::uniformHemisphere();
            ray = {its.point, toWorld(omegaI)};
            throughput *=
                material.eval(omegaO, omegaI) * omegaI.z * (1.0f / Sampler::uniformHemispherePdf());
        }
        else {
            if (!material.isNull())
                wasRough = false;

            // on these surfaces, the outgoing ray direction is fixed - we cannot sample it
            const auto [omegaI, fresnel] = specularReflection(material, omegaO);
            ray = {its.point, toWorld(omegaI)};
            throughput *= fresnel;

            // we might have entered or left a medium
            if (material.containsMedium()) {
                if (omegaI.z >= 0.0f)
                    currentMedium = {};
                else
                    currentMedium = material.medium;
            }
        }

        if (throughput.isBlack())
            break;

        its = {scene, ray};
    }

    result.a = 1.0f;

    return result;
}
