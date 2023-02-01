#include <render/gl/pbr_shader.h>

#include <string>

using namespace std::string_literals;

static const std::string vertex_shader_shadow = R"(
#version 330

uniform mat4 mvp;
in vec3 position;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
}
)"s;
static const std::string fragment_shader_shadow = R"(
#version 330

uniform vec3 albedo;
out vec3 color;

void main() {
    color = albedo;
}
)"s;

PBRShader::PBRShader(const GLMesh& mesh, const Matrix4D& model, const Material& material)
    : model{model}, mesh{mesh}, meshShader{readShaderFile("../src/shaders/pbr.vert"),
                                           readShaderFile("../src/shaders/pbr.frag"),
                                           readShaderFile("../src/shaders/pbr.geom")},
      shadowShader{vertex_shader_shadow, fragment_shader_shadow}
{
    if (material.textures.displacement) {
        meshShader = {
            readShaderFile("../src/shaders/pbr.vert"), readShaderFile("../src/shaders/pbr.frag"),
            readShaderFile("../src/shaders/pbr.geom"), readShaderFile("../src/shaders/pbr.tcs"),
            readShaderFile("../src/shaders/pbr.tes")};
        tesselate = true;
    }

    meshShader.activate();
    meshShader.setVertexAttribute<Point3D>("position", mesh.vertexBuffer);
    meshShader.setVertexAttribute<Normal3D>("normal", mesh.vertexBuffer);
    if (mesh.hasTexCoords) {
        if (material.isTextured()) {
            meshShader.setVertexAttribute<Point2D>("texCoords", mesh.vertexBuffer);
            meshShader.setUniform("textured", true);
            if (tesselate)
                meshShader.setUniform("displacementScale", material.textures.displacementScale);
            meshShader.setUniform("useMetalTexture", !!material.textures.metal);
        }
    }

    meshShader.setUniform("F0", material.albedo());
    meshShader.setUniform("alpha", material.isDiffuse() ? 1.0f : 0.0f);
    specular = !material.isRough();

    meshShader.setUniform("albedo", material.albedo());
    // no diffuse reflection on conductors
    if (material.isRoughConductor() || material.isConductor())
        meshShader.setUniform("albedo", ::Color{0.0f});

    if (material.isRoughConductor()) {
        auto roughConductor = material.roughConductor();
        meshShader.setUniform("alpha", roughConductor.microfacetDistribution.alpha);
        meshShader.setUniform("F0", Point3D{roughConductor.conductor.fresnel(1.0f)});
    }
    else if (material.isRoughPlastic()) {
        auto roughPlastic = material.roughPlastic();
        meshShader.setUniform("alpha", roughPlastic.microfacetDistribution.alpha);
        meshShader.setUniform("F0", Point3D{roughPlastic.dielectric.fresnel(1, 1)});
    }

    meshShader.deactivate();

    shadowShader.activate();
    shadowShader.setUniform("albedo", material.albedo()); // just for debugging :)
    shadowShader.setVertexAttribute<Point3D>("position", mesh.vertexBuffer);
    shadowShader.deactivate();

    if (material.textures.albedo)
        albedoTexture = {material.textures.albedo};
    if (material.textures.normal)
        normalTexture = {material.textures.normal};
    if (material.textures.roughness)
        roughnessTexture = {material.textures.roughness};
    if (material.textures.metal)
        metalTexture = {material.textures.metal};
    if (material.textures.displacement)
        displacementTexture = {material.textures.displacement};
}

void PBRShader::draw(const Light::Point& light, const Point3D cameraPos, const Matrix4D& vp,
                     const Matrix4D& lp, const GLTexture& shadowMap, const GLEnvironmentMap& envmap,
                     const Parameters& params)
{
    GLenum mode{GL_TRIANGLES};

    meshShader.activate();

    meshShader.setUniform("mvp", vp * model);
    meshShader.setUniform("mlp", lp * model);
    meshShader.setUniform("model", model);
    meshShader.setUniform("cameraPos", cameraPos);
    meshShader.setUniform("pointLightPos", light.pos);
    meshShader.setUniform("pointLightPower", light.power);
    meshShader.setUniform("useNormalMapping", params.normalMap);
    meshShader.setUniform("useDisplacementMapping", params.displacementMap);
    meshShader.setUniform("useShadowMapping", params.shadowMap);

    glActiveTexture(GL_TEXTURE0);
    shadowMap.bind();
    meshShader.setUniform("shadowMap", 0);

    glActiveTexture(GL_TEXTURE1);
    albedoTexture.bind();
    meshShader.setUniform("albedoTexture", 1);

    glActiveTexture(GL_TEXTURE2);
    normalTexture.bind();
    meshShader.setUniform("normalMap", 2);

    glActiveTexture(GL_TEXTURE3);
    roughnessTexture.bind();
    meshShader.setUniform("roughnessTexture", 3);

    glActiveTexture(GL_TEXTURE4);
    metalTexture.bind();
    meshShader.setUniform("roughnessTexture", 4);

    if (tesselate) {
        glActiveTexture(GL_TEXTURE5);
        displacementTexture.bind();
        meshShader.setUniform("displacementMap", 5);

        mode = GL_PATCHES;
    }

    if (params.envmap) {
        glActiveTexture(GL_TEXTURE6);
        envmap.envmap.bind();
        meshShader.setUniform("envmap", 6);

        glActiveTexture(GL_TEXTURE7);
        envmap.brdfLUT.bind();
        meshShader.setUniform("brdfLUT", 7);
        meshShader.setUniform("envmapMaxLevel", envmap.maxMipLevel);
    }
    else {
        meshShader.setUniform("envmapMaxLevel", 0);
    }

    // overwrite alpha value given by the material
    if (specular)
        meshShader.setUniform("alpha", params.alpha);

    meshShader.setUniform("shadeFlat", true);
    mesh.drawFlatParts(mode);
    meshShader.setUniform("shadeFlat", false);
    mesh.drawSmoothParts(mode);

    meshShader.deactivate();
}

void PBRShader::drawShadow(const Matrix4D& lp)
{
    shadowShader.activate();
    shadowShader.setUniform("mvp", lp * model);
    mesh.drawAll();
    shadowShader.deactivate();
}
