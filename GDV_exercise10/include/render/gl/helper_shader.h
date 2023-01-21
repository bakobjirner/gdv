#ifndef HELPER_SHADER_H
#define HELPER_SHADER_H

#include <geometry/matrix4d.h>
#include "gl_utils.h"

struct Instance;

class BackgroundShader final {
public:
    BackgroundShader();
    void draw(Point2D framebufferSize);

private:
    GLShaderProgram backgroundShader;
    GLVertexBuffer vertexBuffer{};
};

class BVHShader final {
public:
    BVHShader(const Instance& instance);
    void draw(uint32_t bvhLevel, const Matrix4D& vp);

    /// toWorld transformation matrix
    Matrix4D model;

private:
    size_t numBVHNodes{0};
    GLShaderProgram bvhShader;
    GLVertexBuffer vertexBuffer{};
};

#endif // HELPER_SHADER_H
