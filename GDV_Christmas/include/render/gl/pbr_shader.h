#ifndef PBR_SHADER_H
#define PBR_SHADER_H

#include <render/light.h>
#include <render/material.h>
#include <render/texture.h>
#include "gl_utils.h"
#include "gl_mesh.h"
#include "gl_envmap.h"

#include <nanogui/opengl.h>

#include <vector>
#include <cstdint>

/// shader for physically-based rendering
class PBRShader final {
public:
    struct Parameters {
        bool normalMap{true};
        bool displacementMap{true};
        bool shadowMap{true};
        bool envmap{true};
        float alpha{0.0f};
    };

    PBRShader(const GLMesh& mesh, const Matrix4D& model, const Material& material);
    void draw(const Light::Point& light, const Point3D cameraPos, const Matrix4D& vp,
              const Matrix4D& lp, const GLTexture& shadowMap, const GLEnvironmentMap& envmap,
              const Parameters& params);
    void drawShadow(const Matrix4D& lp);

    /// toWorld transformation matrix
    Matrix4D model;

private:
    const GLMesh& mesh;
    GLShaderProgram meshShader;
    GLShaderProgram shadowShader;
    bool tesselate{false};
    bool specular{false};

    GLTexture albedoTexture;
    GLTexture normalTexture;
    GLTexture roughnessTexture;
    GLTexture metalTexture;
    GLTexture displacementTexture;
};

#endif // PBR_SHADER_H
