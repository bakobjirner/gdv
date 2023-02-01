#ifndef SIMPLE_SHADER_H
#define SIMPLE_SHADER_H

#include <render/light.h>
#include <render/material.h>

#include "gl_utils.h"
#include "gl_mesh.h"

#include <nanogui/opengl.h>

#include <array>
#include <string>

/// basic 3D mesh shader with some debugging views
class SimpleShader final {
public:
    enum class DebugView { None, Depth, Position, Normal, UV };

    SimpleShader(const GLMesh& mesh, const Matrix4D& model, const Material& material);
    void draw(const Light::Point& light, const Point3D cameraPos, const Matrix4D& vp,
              DebugView debugView=DebugView::None, AABB bounds={},
              bool wobble=false, float time=0.0f);

    /// toWorld transformation matrix
    Matrix4D model;

private:
    const GLMesh& mesh;
    GLShaderProgram meshShader;
    GLShaderProgram wobbleShader;
    GLShaderProgram debugShader;

    static const std::string vertex_shader_debug;
    static const std::string fragment_shader_debug;
    static const std::string fragment_shader_wobble;
    static const std::string vertex_shader_wobble;
};

/// simple shader program
class MyLittleShader final {
public:
    MyLittleShader();
    ~MyLittleShader();

    MyLittleShader(const MyLittleShader&) = delete;
    MyLittleShader& operator=(const MyLittleShader&) = delete;
    MyLittleShader(MyLittleShader&&) = delete;
    MyLittleShader& operator=(MyLittleShader&&) = delete;

    void draw(float time);

private:
    /// the shader program
    GLuint program{};
    /// the vertex array representing the mapping of buffers to the shader program parameters
    GLuint vertexArray{};
    /// some buffers to work with
    std::array<GLuint, 4> buffers{};

    // defined in exercise06.cpp
    static const std::string my_little_vertex_shader;
    static const std::string my_little_fragment_shader;
};

#endif // SIMPLE_SHADER_H
