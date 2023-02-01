#include <nanogui/opengl.h>
#include <render/gl/helper_shader.h>

#include <render/instance.h>

#include <string>

using namespace std::string_literals;

static const std::string vertex_shader_flat = R"(
#version 330

uniform mat4 mvp;
in vec3 position;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
}
)"s;
static const std::string fragment_shader_flat = R"(
#version 330

uniform vec3 albedo;
out vec3 color;

void main() {
    color = albedo;
}
)"s;

static const std::string vertex_shader_checkers = R"(
#version 330

uniform mat4 matrix_background;
in vec2 position;
out vec2 position_background;

void main() {
    vec4 p = vec4(position, 0.0, 1.0);
    gl_Position = p;
    position_background = (matrix_background * p).xy;
}
)"s;

static const std::string fragment_shader_checkers = R"(
#version 330

in vec2 position_background;
out vec3 frag_color;

void main() {
    vec2 frac = position_background - floor(position_background);
    float checkerboard = ((frac.x > .5) == (frac.y > .5)) ? 0.4 : 0.5;
    frag_color = vec3(checkerboard);
}
)"s;

static const Point2D fullScreenQuad[]{{-1.f, -1.f}, {1.f, -1.f}, {-1.f, 1.f},
                                      {1.f, -1.f},  {1.f, 1.f},  {-1.f, 1.f}};

BackgroundShader::BackgroundShader()
    : backgroundShader{vertex_shader_checkers, fragment_shader_checkers}
{
    vertexBuffer.setBuffer<Point2D>("position", fullScreenQuad);
    backgroundShader.activate();
    backgroundShader.setVertexAttribute<Point2D>("position", vertexBuffer);
    backgroundShader.deactivate();
}

void BackgroundShader::draw(Point2D framebufferSize)
{
    const HomogeneousTransformation3D matrixBackground{
        Matrix3D::scale({framebufferSize.x / 40.f, framebufferSize.y / 40.f, 1.f}),
        {-framebufferSize.x / 40.f, -framebufferSize.y / 40.f, 0.f}};
    backgroundShader.activate();
    backgroundShader.setUniform("matrix_background", Matrix4D{matrixBackground});
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
    backgroundShader.deactivate();
}

BVHShader::BVHShader(const Instance& instance)
    : model{instance.toWorld}, bvhShader{vertex_shader_flat, fragment_shader_flat}
{
    const Mesh& mesh = instance.mesh;

    const BVH& bvh = mesh.getBVH();
    const auto& bvhNodes = bvh.getNodes();
    numBVHNodes = bvhNodes.size();
    {
        std::vector<Point3D> bvhVertices;
        bvhVertices.reserve(numBVHNodes * 8);
        for (auto& node : bvhNodes) {
            bvhVertices.emplace_back(node.bounds.min.x, node.bounds.min.y, node.bounds.min.z);
            bvhVertices.emplace_back(node.bounds.min.x, node.bounds.min.y, node.bounds.max.z);
            bvhVertices.emplace_back(node.bounds.min.x, node.bounds.max.y, node.bounds.min.z);
            bvhVertices.emplace_back(node.bounds.min.x, node.bounds.max.y, node.bounds.max.z);
            bvhVertices.emplace_back(node.bounds.max.x, node.bounds.min.y, node.bounds.min.z);
            bvhVertices.emplace_back(node.bounds.max.x, node.bounds.min.y, node.bounds.max.z);
            bvhVertices.emplace_back(node.bounds.max.x, node.bounds.max.y, node.bounds.min.z);
            bvhVertices.emplace_back(node.bounds.max.x, node.bounds.max.y, node.bounds.max.z);
        }
        vertexBuffer.setBuffer<Point3D>("position", {bvhVertices.data(), bvhVertices.size()});
    }
    {
        std::vector<LineIndices> bvhIndices;
        bvhIndices.reserve(numBVHNodes * 24);
        for (uint32_t i = 0; i < numBVHNodes; ++i) {
            // base
            bvhIndices.push_back({8 * i + 0, 8 * i + 1});
            bvhIndices.push_back({8 * i + 1, 8 * i + 3});
            bvhIndices.push_back({8 * i + 3, 8 * i + 2});
            bvhIndices.push_back({8 * i + 2, 8 * i + 0});

            // side
            bvhIndices.push_back({8 * i + 0, 8 * i + 4});
            bvhIndices.push_back({8 * i + 1, 8 * i + 5});
            bvhIndices.push_back({8 * i + 2, 8 * i + 6});
            bvhIndices.push_back({8 * i + 3, 8 * i + 7});

            // top
            bvhIndices.push_back({8 * i + 4, 8 * i + 5});
            bvhIndices.push_back({8 * i + 5, 8 * i + 7});
            bvhIndices.push_back({8 * i + 7, 8 * i + 6});
            bvhIndices.push_back({8 * i + 6, 8 * i + 4});
        }
        vertexBuffer.setBuffer<LineIndices>("lines", {bvhIndices.data(), bvhIndices.size()});
    }

    bvhShader.activate();
    bvhShader.setUniform("albedo",
                         ::Color{nanogui::Color{instance.material.albedo()}.contrasting_color()});
    bvhShader.setVertexAttribute<Point3D>("position", vertexBuffer);
    bvhShader.deactivate();
}

void BVHShader::draw(uint32_t bvhLevel, const Matrix4D& vp)
{
    bvhShader.activate();
    bvhShader.setUniform("mvp", vp * model);

    const size_t displayBVHNodes =
        std::min(numBVHNodes, static_cast<size_t>(1UL << bvhLevel) - 1UL);
    vertexBuffer.draw("lines", GL_LINES, 0, displayBVHNodes * 12);

    bvhShader.deactivate();
}
