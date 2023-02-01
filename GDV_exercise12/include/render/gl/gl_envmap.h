#ifndef GL_ENVMAP_H
#define GL_ENVMAP_H

#include <geometry/point2d.h>
#include <geometry/point3d.h>
#include <geometry/matrix4d.h>
#include <render/texture.h>
#include "gl_utils.h"

struct GLEnvironmentMap
{
    GLEnvironmentMap() = default;
    GLEnvironmentMap(const Texture& envmapTexture);

    /// number of samples per pixel for pre-filtering
    static const uint32_t numSamples = 512;

    /// sample GGX microfacet distribution (preserves low-discrepancy sampling pattern)
    static Vector3D sampleGGX(Point2D sample, float alpha);

    /// pre-filtered environment map
    GLTexture envmap{};
    /// pre-filtered BRDF lookup texture
    GLTexture brdfLUT{};
    /// number of mip-map levels in the pre-filtered environment map
    GLint maxMipLevel{0};
};

class EnvmapShader {
public:
    EnvmapShader();
    void draw(const GLEnvironmentMap& envmap, const Matrix4D& view);
private:
    GLShaderProgram envmapShader;
    GLVertexBuffer vertexBuffer{};
};

#endif // GL_ENVMAP_H
