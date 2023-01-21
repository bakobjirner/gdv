#include "common/constants.h"
#include "geometry/point3d.h"
#include "render/color.h"
#include <render/raytracer.h>
#include <render/spheretracer.h>

Color Material::Absorption::transmittance(float distance) const
{
    return exp(-(Color(distance) * absorptionCoefficient));
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
    while(remainingDist>0 && dist>=epsilon){
        pos = pos + ray.direction * dist;
        remainingDist = remainingDist-dist;
        dist = eval(pos);
        i++;
    }
    return {pos, i};
}

Normal3D SignedDistanceFunction::computeNormal(Point3D pos) const
{
    // TODO: compute the normal numerically using central differences
    // while the gradient of the SDF should have magnitude 1 everywhere,
    // it should be easier and safer to simply normalize the resulting vector before returning it
    float x = eval(pos+Point3D(epsilon,0,0))-eval(pos-Point3D(epsilon,0,0));
    float y = eval(pos+Point3D(0,epsilon,0))-eval(pos-Point3D(0,epsilon,0));
    float z = eval(pos+Point3D(0,0,epsilon))-eval(pos-Point3D(0,0,epsilon));
    return normalize(Point3D(x,y,z)); // replace this with your result
}
