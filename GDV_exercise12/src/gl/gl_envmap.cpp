#include <render/gl/gl_envmap.h>

#include <render/material.h>
#include <render/sampler.h>

#include <array>
#include <bit>
#include <iostream>
#include <string>

using namespace std::string_literals;

static const std::string vertex_shader_envmap = R"(
#version 330

uniform mat4 view;
in vec2 position;
out vec3 viewDir;

void main() {
    vec4 p = vec4(position, 0.0, 1.0);
    viewDir = transpose(mat3(view)) * vec3(position, -1.0);
    gl_Position = p;
}
)"s;

static const std::string fragment_shader_envmap = R"(
#version 330

uniform sampler2D envmap;
in vec3 viewDir;
out vec3 color;

const float pi = 3.14159265359f;

void main() {
    vec3 dir = normalize(viewDir);

    float theta = acos(dir.y);
    float phi = atan(-dir.x, dir.z);

    color = textureLod(envmap, vec2(phi/(2.0f*pi)+0.5f, theta/pi), 0).rgb;
}
)"s;

static const Point2D fullScreenQuad[]{{-1.f, -1.f}, {1.f, -1.f}, {-1.f, 1.f},
                                      {1.f, -1.f},  {1.f, 1.f},  {-1.f, 1.f}};

GLEnvironmentMap::GLEnvironmentMap(const Texture& envmapTexture) : envmap{envmapTexture}
{
    if (envmapTexture.dataType == Texture::DataType::Float
        && envmapTexture.channels == Texture::Channels::RGB) {
        // generate pre-filtered mip-map

        // lowest level should have at least 8 pixels in the smaller dimension, so we discard 4 bits
        /// number of mip-map levels
        maxMipLevel = 32 - 4
                    - std::countl_zero(static_cast<uint32_t>(
                        std::min(envmapTexture.resolution.x, envmapTexture.resolution.y)));

        /// ggx samples for the current roughness value
        std::array<Vector3D, numSamples> ggxSamples;

#if defined(_WIN32)
        using OMPIndex = int32_t;
#else
        using OMPIndex = uint32_t;
#endif

        envmap.bind();

        std::cout << "Prefiltering the environment map..." << std::flush;

        for (GLint mipLevel = 1; mipLevel <= maxMipLevel; ++mipLevel) {
            /// resolution of this mip map level
            Resolution mipRes;
            {
                // ask OpenGL for the current mip-map resolution
                GLint width, height;
                glGetTexLevelParameteriv(GL_TEXTURE_2D, mipLevel, GL_TEXTURE_WIDTH, &width);
                glGetTexLevelParameteriv(GL_TEXTURE_2D, mipLevel, GL_TEXTURE_HEIGHT, &height);
                mipRes = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
            }

            // fetch texture down-sampled by OpenGL for this level (to prevent aliasing)
            Texture inputTexture{mipRes, Texture::Channels::RGB, Texture::DataType::Float};
            glGetTexImage(GL_TEXTURE_2D, mipLevel, GL_RGB, GL_FLOAT, inputTexture.data.get());

            /// replacement texture for this mip-level with pre-filtered roughness
            Texture mipTexture{mipRes, Texture::Channels::RGB, Texture::DataType::Float};
            // linear roughness levels
            const float alphaSqrt = static_cast<float>(mipLevel) / static_cast<float>(maxMipLevel);
            // square-root-distributed roughness levels (low roughness values are more important)
            const float alpha = alphaSqrt * alphaSqrt;

            const auto inputPixels = inputTexture.getData<Vector3D>();
            auto outputPixels = mipTexture.getData<Vector3D>();

            float normalization = 0.0f;

#pragma omp parallel for reduction(+ : normalization)
            for (OMPIndex i = 0; i < numSamples; ++i) {
                ggxSamples[i] = sampleGGX(Sampler::halton(i), alpha);
                normalization += ggxSamples[i].z;
            }

            normalization = 1.0f / normalization;

            const float thetaScale = pi / static_cast<float>(mipRes.y);
            const float phiScale = 2.0f * pi / static_cast<float>(mipRes.x);

#pragma omp parallel for
            for (OMPIndex y = 0; y < mipRes.y; ++y) {
                const float theta = (static_cast<float>(y) + 0.5f) * thetaScale;
                const float sinTheta = std::sin(theta);
                const float cosTheta = std::cos(theta);
                for (uint32_t x = 0; x < mipRes.x; ++x) {
                    const float phi = (static_cast<float>(x) + 0.5f) * phiScale;
                    const float sinPhi = std::sin(phi);
                    const float cosPhi = std::cos(phi);
                    /// normal for this pixel in the environment map frame (in world space, y is up)
                    const Normal3D n{sinTheta * sinPhi, cosTheta, -sinTheta * cosPhi};
                    const OrthonormalSystem frame{n};

                    Vector3D sum{};

                    for (uint32_t i = 0; i < numSamples; ++i) {
                        /// sample from GGX microfacet distribution centered around the normal
                        const Vector3D omegaI = frame.toWorld(ggxSamples[i]);
                        /// weight each sample by the cosine term
                        const float weight = ggxSamples[i].z;

                        const float thetaI = std::acos(omegaI.y);
                        const float phiI = std::atan2(-omegaI.x, omegaI.z) + pi;

                        uint32_t pixelY =
                            static_cast<uint32_t>(static_cast<float>(mipRes.y) * thetaI * invPi);
                        uint32_t pixelX =
                            static_cast<uint32_t>(static_cast<float>(mipRes.x) * phiI * invTwoPi);
                        if (pixelX >= mipRes.x) // atan2 can return pi, so this might overflow
                            pixelX -= mipRes.x; // here we wrap around left to right
                        if (pixelY >= mipRes.y) // acos can return pi, so this might overflow
                            pixelY = mipRes.y - 1; // here we clamp to top and bottom

                        sum += inputPixels[pixelY * mipRes.x + pixelX] * weight;
                    }

                    outputPixels[y * mipRes.x + x] = sum * normalization;
                }
            }

            // report progress
            std::cout << " " << mipLevel << "..." << std::flush;

            // replace mip-level texture
            GLint resX = static_cast<GLint>(mipRes.x);
            GLint resY = static_cast<GLint>(mipRes.y);
            glTexImage2D(GL_TEXTURE_2D, mipLevel, GL_RGB, resX, resY, 0, GL_RGB, GL_FLOAT,
                         mipTexture.data.get());

            CHECK_GL();
        }

        // don't interpolate between top and bottom
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        envmap.unbind();

        {
            // compute the pre-filtered BRDF lookup table

            std::cout << " LUT..." << std::flush;

            const Resolution lutRes{512, 512};
            Texture brdfLUT{lutRes, Texture::Channels::RG, Texture::DataType::Float};
            auto outputPixels = brdfLUT.getData<Vector2D>();
            const float invNumSamples = 1.0f / static_cast<float>(numSamples);

            for (uint32_t y = 0; y < lutRes.y; ++y) {
                // linear roughness levels
                const float roughness = static_cast<float>(y) / static_cast<float>(lutRes.y);
                // compute mip levels for alpha=roughness*roughness
                // (low roughness values are more important)
                const float alpha = roughness * roughness;

#pragma omp parallel for
                for (OMPIndex i = 0; i < numSamples; ++i) {
                    ggxSamples[i] = sampleGGX(Sampler::halton(i), alpha);
                }

                const Material::GGX microfacetDistribution{alpha};

#pragma omp parallel for
                for (OMPIndex x = 0; x < lutRes.x; ++x) {
                    const float cosThetaO = static_cast<float>(x) / static_cast<float>(lutRes.x);
                    const float sinThetaO = std::sqrt(1.0f - cosThetaO * cosThetaO);
                    const float G1O = microfacetDistribution.G1(cosThetaO);

                    Vector2D sum{};

                    for (uint32_t i = 0; i < numSamples; ++i) {
                        const Normal3D& micronormal = ggxSamples[i];
                        const float cosThetaM = micronormal.z;
                        // dot-product of micronormal and omegaO (omegaO.y = 0)
                        const float cosThetaOM =
                            sinThetaO * micronormal.x + cosThetaO * micronormal.z;
                        // reflect omegaO at the micronormal (only compute the z component)
                        const float cosThetaI = cosThetaM * (2.0f * cosThetaOM) - cosThetaO;

                        if (cosThetaI > 0.0f) {
                            // shadowing term G(cosThetaI, cosThetaO) = G1(cosThetaI)*G1(cosThetaO)
                            const float G1I = microfacetDistribution.G1(cosThetaI);
                            // shadowing term, geometry terms, and inverse PDF of GGX sampling
                            const float G_Vis = G1I * G1O * cosThetaOM / (cosThetaM * cosThetaO);
                            const float u = 1.0f - cosThetaOM;
                            const float uSqr = u * u;
                            const float fresnelSchlick = uSqr * uSqr * u;
                            sum += Vector2D{(1.0f - fresnelSchlick), fresnelSchlick} * G_Vis;
                        }
                    }

                    outputPixels[y * lutRes.x + x] = sum * invNumSamples;
                }
            }

            // copy data from the Texture into a GLTexture
            this->brdfLUT = {brdfLUT};

            this->brdfLUT.bind();
            // dont interpolate across edges
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            this->brdfLUT.unbind();
        }

        std::cout << " done.\n" << std::flush;
    }
    else {
        std::cout << "Prefiltering the environment map is only implemented for RGB float textures."
                  << std::endl;
    }
}

Vector3D GLEnvironmentMap::sampleGGX(Point2D sample, float alpha)
{
    /* Modified concentric map code with less branching (by Dave Cline), see
       http://psgraphics.blogspot.ch/2011/01/improved-code-for-concentric-map.html
    */
    sample = sample * 2.0f - 1.0f;
    float phi, r;
    if (sample.x == 0.0f && sample.y == 0.0f) {
        r = phi = 0.0f;
    }
    else if (sample.x * sample.x > sample.y * sample.y) {
        r = sample.x;
        phi = (pi * 0.25f) * (sample.y / sample.x);
    }
    else {
        r = sample.y;
        phi = (pi * 0.5f) - (sample.x / sample.y) * (pi * 0.25f);
    }

    const float sinPhi = std::sin(phi) * std::copysign(1.0f, r);
    const float cosPhi = std::cos(phi) * std::copysign(1.0f, r);

    const float rSqr = r * r; // this value is uniformly distributed between 0 and 1

    // GGX microfacet distribution
    const float cosThetaSqr = (1.0f - rSqr) / (1.0f + rSqr * alpha * alpha - rSqr);
    const float sinTheta = std::sqrt(1.0f - cosThetaSqr);
    const float cosTheta = std::sqrt(cosThetaSqr);

    return {sinTheta * cosPhi, sinTheta * sinPhi, cosTheta};
}

EnvmapShader::EnvmapShader() : envmapShader(vertex_shader_envmap, fragment_shader_envmap)
{
    vertexBuffer.setBuffer<Point2D>("position", fullScreenQuad);
    envmapShader.activate();
    envmapShader.setVertexAttribute<Point2D>("position", vertexBuffer);
    envmapShader.deactivate();
}

void EnvmapShader::draw(const GLEnvironmentMap& envmap, const Matrix4D& view)
{
    envmapShader.activate();
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    envmap.envmap.bind();
    envmapShader.setUniform("envmap", 0);
    envmapShader.setUniform("view", view);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    envmap.envmap.unbind();
    glEnable(GL_DEPTH_TEST);
    envmapShader.deactivate();
}
