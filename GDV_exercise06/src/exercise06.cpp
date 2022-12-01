#include <render/gl_shader.h>
#include <render/gl_utils.h>

#include <string>

using namespace std::string_literals;

const static std::string my_little_vertex_shader = R"(
#version 330

// input/output parameters

uniform float scaling;
in vec3 position;
in vec3 colorIn;
out vec3 color;

void main() {
    gl_Position = vec4(position * scaling, 1.0);
    color = colorIn;
}
)"s;

const static std::string my_little_fragment_shader = R"(
#version 330

//  input/output parameters
in vec3 color;
out vec3 colorOut;

void main() {
    colorOut = color;
}
)"s;

extern const std::string vertex_shader_debug = R"(
#version 330

uniform mat4 mvp;
uniform mat4 model;

in vec3 position;
in vec3 normal;
out vec4 ws_position;
out vec3 ws_normal;
flat out vec3 ws_normal_flat;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    ws_position = model * vec4(position, 1.0);
    ws_normal = transpose(inverse(mat3(model)))*normal;
    ws_normal_flat = ws_normal;
}
)"s;

extern const std::string fragment_shader_debug = R"(
#version 330

uniform int mode; // 0: depth, 1: position, 2: normal
uniform vec3 reference; // depth: camera pos / position: minimum scene point
uniform vec3 extents; // depth/position: scene extents (including/excluding camera)

uniform bool shade_flat;

in vec4 ws_position;
in vec3 ws_normal;
flat in vec3 ws_normal_flat;
out vec3 color;

void main() {
    if (mode == 0) { // depth
        vec3 cameraPos = reference;
        vec3 maxDepth = extents;

        // TODO: compute the depth from the world space position and the camera position
        // TODO: normalize using the maximum depth
        color = vec3(0.0);
    }
    else if (mode == 1) { // position
        vec3 aabbMin = reference;
        vec3 aabbExtents = extents;

        // TODO: set the color to the normalized (in [0,1]^3) world space position
        // TODO: use the minimum and the extents of the scene bounds for normalization
        color = vec3(0.0);
    }
    else if (mode == 2) { // normal
        vec3 normal = normalize(shade_flat ? ws_normal_flat : ws_normal);

        // TODO: set the color to the normalized (in [0,1]^3) world space normal
        color = vec3(0.0);
    }
    else {
        color = vec3(0.0);
    }
}
)"s;

extern const std::string vertex_shader_wobble = R"(
#version 330

uniform mat4 mlp;
uniform mat4 mvp;
uniform mat4 model;

uniform float time;

in vec3 position;
in vec3 normal;
out vec4 ws_position;
out vec3 ws_normal;
flat out vec3 ws_normal_flat;

void main() {
    // TODO: apply the "wobble" transformation as described on the exercise sheet

    gl_Position = mvp * vec4(position, 1.0);
    ws_position = model * vec4(position, 1.0);
    ws_normal = transpose(inverse(mat3(model)))*normal;
    ws_normal_flat = ws_normal;
}
)"s;

// matching vertex shader is implemented in gl_shader.cpp
extern const std::string fragment_shader = R"(
#version 330

uniform vec3 albedo;
uniform bool shade_flat;
uniform vec3 point_light_pos;
uniform vec3 point_light_power;

in vec4 ws_position;
in vec3 ws_normal;
flat in vec3 ws_normal_flat;
out vec3 color;

const float pi = 3.14159265359f;

float srgb(float x) {
    return x < 0.0031308 ? 12.92 * x : (1.055 * pow(x, 1.0/2.4) - 0.055);
}

void main() {
    vec3 normal = normalize(shade_flat ? ws_normal_flat : ws_normal);
    if (gl_FrontFacing) {
        // TODO: compute the diffuse shading
        // use the world space position, world space normal,
        // point light position, and point light power
        // to compute the shading terms

        vec3 ws_pos = ws_position.xyz/ws_position.w;

        // simple shading using ambient light and the world-space normal -- remove this
        color = albedo*(0.25+0.75*max(0, normal.z));

        // convert to sRGB
        color = vec3(srgb(color.r), srgb(color.g), srgb(color.b));
    }
    else {
        color = vec3(0.0);
    }
}
)"s;

MyLittleShader::MyLittleShader()
{
    float mesh[] ={-0.5, 0.5,0, 0.5, 0.5,0, -0.5, -0.5,0, 0.5, 0.5,0, 0.5, -0.5,0, -0.5, -0.5,0};
    float colors[] ={1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};



    program = GLShaderProgram::compileShaderProgram(my_little_vertex_shader,my_little_fragment_shader);
    glCreateVertexArrays(1,&vertexArray);
    glCreateBuffers(1,&buffers[0]);
    glCreateBuffers(1,&buffers[1]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 6, mesh, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 6, colors, GL_STATIC_DRAW);

    //unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(vertexArray);

    //link mesh
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glEnableVertexAttribArray(glGetAttribLocation(program, "position"));
    glVertexAttribPointer(glGetAttribLocation(program, "position"), 3, GL_FLOAT,GL_FALSE, 0, 0);

    //link color
    glBindBuffer(GL_ARRAY_BUFFER,buffers[1]);
    glEnableVertexAttribArray(glGetAttribLocation(program, "colorIn"));
    glVertexAttribPointer(glGetAttribLocation(program, "colorIn"), 3, GL_FLOAT,GL_FALSE, 0, 0);

    // unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // unbind vertexArray
    glBindVertexArray(0);

    CHECK_GL();
}

MyLittleShader::~MyLittleShader()
{
    glDeleteProgram(program);
    glDeleteVertexArrays(1,&vertexArray);
    glDeleteBuffers(1,&buffers[0]);
    glDeleteBuffers(1,&buffers[1]);

    // TODO: cleanup your OpenGL shader program, vertex array, and buffers
    CHECK_GL();
}

void MyLittleShader::draw(float time)
{
    // checking if something went wrong somewhere else
    CHECK_GL();
    glUseProgram(program);
    glUniform1f(glGetUniformLocation(program,"scaling"),0.5);
    glBindVertexArray(vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glUseProgram(0);
    // TODO: draw your OpenGL vertex array
    CHECK_GL();
}
