#include <render/gl/simple_shader.h>

#include <string>

using namespace std::string_literals;

const std::string MyLittleShader::my_little_vertex_shader = R"(
#version 330

uniform float scale;

in vec2 position;
in vec4 color;
out vec3 vsColor;

void main() {
    gl_Position = vec4(position*scale, 0.0, 1.0);
    vsColor = color.rgb;
}
)"s;
const std::string MyLittleShader::my_little_fragment_shader = R"(
#version 330

in vec3 vsColor;
out vec3 fragColor;

void main() {
    fragColor = vsColor;
}
)"s;

const std::string SimpleShader::vertex_shader_debug = R"(
#version 330

uniform mat4 mvp;
uniform mat4 model;

in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec4 wsPosition;
out vec3 wsNormal;
flat out vec3 wsNormalFlat;
out vec2 fragTexCoords;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    wsPosition = model * vec4(position, 1.0);
    wsNormal = transpose(inverse(mat3(model)))*normal;
    wsNormalFlat = wsNormal;
    fragTexCoords = texCoords;
}
)"s;

const std::string SimpleShader::fragment_shader_debug = R"(
#version 330

uniform int mode; // 0: depth, 1: position, 2: normal
uniform vec3 reference; // depth: camera pos / position: minimum scene point
uniform vec3 extents; // depth/position: scene extents (including/excluding camera)

uniform bool shadeFlat;

in vec4 wsPosition;
in vec3 wsNormal;
flat in vec3 wsNormalFlat;
in vec2 fragTexCoords;

out vec3 color;

void main() {
    if (mode == 0) { // depth
        vec3 cameraPos = reference;
        vec3 maxDepth = extents;

        color = length(wsPosition.xyz-cameraPos)/maxDepth;
    }
    else if (mode == 1) { // position
        vec3 aabbMin = reference;
        vec3 aabbExtents = extents;

        color = (wsPosition.xyz-aabbMin)/aabbExtents;
    }
    else if (mode == 2) { // normal
        vec3 normal = normalize(shadeFlat ? wsNormalFlat : wsNormal);

        color = normal*0.5+vec3(0.5);
    }
    else if (mode == 3) { // uv
        color = vec3(fragTexCoords, 0.0f);
    }
    else {
        color = vec3(0.0);
    }
}
)"s;

const std::string SimpleShader::vertex_shader_wobble = R"(
#version 330

uniform mat4 mlp;
uniform mat4 mvp;
uniform mat4 model;

uniform float time;

in vec3 position;
in vec3 normal;
out vec4 wsPosition;
out vec3 wsNormal;
flat out vec3 wsNormalFlat;

void main() {
    vec3 pos;
    pos.x = (1.0+0.25*sin(5.0*(time+position.y)))*position.x;
    pos.y = position.y;
    pos.z = (1.0+0.25*sin(5.0*(time+position.y)))*position.z;

    gl_Position = mvp * vec4(pos, 1.0);
    wsPosition = model * vec4(pos, 1.0);
    wsNormal = transpose(inverse(mat3(model)))*normal;
    wsNormalFlat = wsNormal;
}
)"s;

const std::string SimpleShader::fragment_shader_wobble = R"(
#version 330

uniform vec3 albedo;
uniform bool shadeFlat;
uniform vec3 pointLightPos;
uniform vec3 pointLightPower;

in vec4 wsPosition;
in vec3 wsNormal;
flat in vec3 wsNormalFlat;
out vec3 color;

const float pi = 3.14159265359f;

void main() {
    vec3 normal = normalize(shadeFlat ? wsNormalFlat : wsNormal);
    if (gl_FrontFacing) {
        // diffuse shading
        vec3 wsPos = wsPosition.xyz/wsPosition.w;
        vec3 wsDir = pointLightPos-wsPos;
        float dist = length(wsDir);
        float cosTheta = max(0, dot(normal, normalize(wsDir)));

        color = albedo*pointLightPower*(cosTheta/(dist*dist*(4.0f*pi*pi)));
    }
    else {
        color = vec3(0.0);
    }
}
)"s;

SimpleShader::SimpleShader(const GLMesh& mesh, const Matrix4D& model, const Material& material)
    : model{model}, mesh{mesh}, meshShader{readShaderFile("../src/shaders/pbr_no_geom.vert"),
                                           readShaderFile("../src/shaders/pbr.frag")},
      wobbleShader{vertex_shader_wobble, fragment_shader_wobble}, debugShader{vertex_shader_debug,
                                                                              fragment_shader_debug}
{
    meshShader.activate();
    meshShader.setVertexAttribute<Point3D>("position", mesh.vertexBuffer);
    meshShader.setVertexAttribute<Normal3D>("normal", mesh.vertexBuffer);

    if (material.isRoughConductor()) {
        auto roughConductor = material.roughConductor();
        meshShader.setUniform("albedo", ::Color{0.0f});
        meshShader.setUniform("alpha", roughConductor.microfacetDistribution.alpha);
        meshShader.setUniform("F0", Point3D{roughConductor.conductor.fresnel(1.0f)});
    }
    else if (material.isRoughPlastic()) {
        auto roughPlastic = material.roughPlastic();
        meshShader.setUniform("albedo", roughPlastic.diffuse.albedo);
        meshShader.setUniform("alpha", roughPlastic.microfacetDistribution.alpha);
        meshShader.setUniform("F0", Point3D{roughPlastic.dielectric.fresnel(1, 1)});
    }
    else {
        meshShader.setUniform("albedo", material.albedo());
        meshShader.setUniform("alpha", 1.0f);
    }

    meshShader.deactivate();

    wobbleShader.activate();
    wobbleShader.setUniform("albedo", material.albedo());
    wobbleShader.setVertexAttribute<Point3D>("position", mesh.vertexBuffer);
    wobbleShader.setVertexAttribute<Normal3D>("normal", mesh.vertexBuffer);
    wobbleShader.deactivate();

    debugShader.activate();
    debugShader.setVertexAttribute<Point3D>("position", mesh.vertexBuffer);
    debugShader.setVertexAttribute<Normal3D>("normal", mesh.vertexBuffer);
    if (mesh.hasTexCoords)
        debugShader.setVertexAttribute<Point2D>("texCoords", mesh.vertexBuffer);
    debugShader.deactivate();
}

void SimpleShader::draw(const Light::Point& light, const Point3D cameraPos, const Matrix4D& vp,
                        DebugView debugView, AABB bounds, bool wobble, float time)
{
    GLShaderProgram& shader =
        debugView != DebugView::None ? debugShader : (wobble ? wobbleShader : meshShader);

    shader.activate();

    shader.setUniform("mvp", vp * model);
    shader.setUniform("model", model);
    if (debugView != DebugView::None) {
        shader.setUniform("mode", static_cast<int32_t>(debugView) - 1);

        // extra parameters for depth and position modes
        if (debugView == DebugView::Depth) {
            bounds.extend(cameraPos);
            debugShader.setUniform("reference", cameraPos);
            debugShader.setUniform("extents", Vector3D{bounds.extents().maxComponent()});
        }
        else if (debugView == DebugView::Position) {
            debugShader.setUniform("reference", bounds.min);
            debugShader.setUniform("extents", bounds.extents());
        }
    }
    else {
        shader.setUniform("pointLightPos", light.pos);
        shader.setUniform("pointLightPower", light.power);
        if (wobble)
            shader.setUniform("time", time);
        else
            shader.setUniform("cameraPos", cameraPos);
    }

    shader.setUniform("shadeFlat", true);
    mesh.drawFlatParts();
    shader.setUniform("shadeFlat", false);
    mesh.drawSmoothParts();

    shader.deactivate();
}

MyLittleShader::MyLittleShader()
{
    program =
        GLShaderProgram::compileShaderProgram(my_little_vertex_shader, my_little_fragment_shader);
    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(static_cast<GLsizei>(buffers.size()), buffers.data());
    CHECK_GL();

    const Point2D positions[]{
        {-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, 1.0f}, {-1.0f, -1.0f},
    };
    const Color colors[]{{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f},
                         {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}};

    glBindVertexArray(vertexArray);
    CHECK_GL();

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    GLuint pos = static_cast<GLuint>(glGetAttribLocation(program, "position"));
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, sizeof(Point2D) / sizeof(float), GL_FLOAT, GL_FALSE, 0, 0);
    CHECK_GL();

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    GLuint col = static_cast<GLuint>(glGetAttribLocation(program, "color"));
    glEnableVertexAttribArray(col);
    glVertexAttribPointer(col, sizeof(Color) / sizeof(float), GL_FLOAT, GL_FALSE, 0, 0);
    CHECK_GL();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL();
}

MyLittleShader::~MyLittleShader()
{
    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vertexArray);
    glDeleteBuffers(static_cast<GLsizei>(buffers.size()), buffers.data());
    CHECK_GL();
}

void MyLittleShader::draw(float time)
{
    // checking if something went wrong somewhere else
    CHECK_GL();

    // glDisable(GL_CULL_FACE);
    // glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    glBindVertexArray(vertexArray);
    GLint scale = glGetUniformLocation(program, "scale");
    glUniform1f(scale, 0.5f + std::sin(time) * 0.25f);
    CHECK_GL();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    CHECK_GL();
    glUseProgram(0);
    glBindVertexArray(0);
    CHECK_GL();
}
