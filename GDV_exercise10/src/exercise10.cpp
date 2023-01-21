#include <render/raytracer.h>
#include <render/spheretracer.h>

Color Material::Absorption::transmittance(float distance) const
{
    // TODO: compute the transmittance for a ray traveling the given distance through this medium
    return (Color{2.0f} - absorptionCoefficient / Point3D{absorptionCoefficient}.maxComponent())
         * 0.5f; // replace this with your implementation
}

std::pair<Point3D, uint32_t> SignedDistanceFunction::sphereTrace(Ray ray) const
{
    /// current position
    /// starting at tMin
    Point3D pos = ray.origin + ray.direction * ray.tMin;
    /// remaining allowed distance along the ray
    float remainingDist = ray.tMax - ray.tMin;
    /// current distance to the implicit surface
    float dist = eval(pos);

    /// number of sphere tracing iterations
    uint32_t i = 0;

    // TODO: implement the sphere tracing algorithm
    // you can stop the trace once distance <= epsilon (hit)
    // or when you have exceeded the allowed distance of the ray tMax (miss)

    return {pos, i};
}

Normal3D SignedDistanceFunction::computeNormal(Point3D pos) const
{
    // TODO: compute the normal numerically using central differences
    // while the gradient of the SDF should have magnitude 1 everywhere,
    // it should be easier and safer to simply normalize the resulting vector before returning it
    return {}; // replace this with your result
}
