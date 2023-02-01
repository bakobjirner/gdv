#include <nanogui/opengl.h>
#include <render/gl/post_processing_shader.h>

#include <render/instance.h>

#include <string>

using namespace std::string_literals;

static const std::string vertexShaderFlat = R"(
#version 330

in vec2 position;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
}
)"s;

const std::string CompositionShader::fragmentShaderComposition = R"(
#version 330

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;
uniform sampler2D blurTexture;

uniform float minDepth;
uniform float maxDepth;
uniform float blurDepth;

out vec3 color;

float srgb(float x) {
    return x < 0.0031308 ? 12.92 * x : (1.055 * pow(x, 1.0/2.4) - 0.055);
}

void main() {
    ivec2 texSize = textureSize(colorTexture, 0);
    vec2 texCoord = gl_FragCoord.xy/texSize;

    float depth = texture(depthTexture, texCoord).r;
    // undo depth projection
    // https://stackoverflow.com/questions/6652253/getting-the-true-z-value-from-the-depth-buffer
    depth = 2.0f*minDepth*maxDepth / (maxDepth+minDepth - (depth*2.0f-1.0f)*(maxDepth-minDepth));

    color = texture(colorTexture, texCoord).rgb;

    if (depth > blurDepth) {
        vec3 colorBlur = texture(blurTexture, texCoord).rgb;
        color = mix(color, colorBlur, min(1.0f, sqrt(depth/blurDepth-1.0f)));
    }

    // convert to sRGB
    color = vec3(srgb(color.r), srgb(color.g), srgb(color.b));
}
)"s;

static const Point2D fullScreenQuad[]{{-1.f, -1.f}, {1.f, -1.f}, {-1.f, 1.f},
                                      {1.f, -1.f},  {1.f, 1.f},  {-1.f, 1.f}};

BlurShader::BlurShader() : blurShader{vertexShaderFlat, fragmentShaderBlur}
{
    vertexBuffer.setBuffer<Point2D>("position", fullScreenQuad);
    blurShader.activate();
    blurShader.setVertexAttribute<Point2D>("position", vertexBuffer);
    blurShader.deactivate();
}

void BlurShader::draw(const GLTexture& color, bool horizontal)
{
    blurShader.activate();
    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    color.bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    blurShader.setUniform("colorTexture", 0);

    blurShader.setUniform("horizontal", horizontal);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glEnable(GL_DEPTH_TEST);
    blurShader.deactivate();
}

CompositionShader::CompositionShader()
    : compositionShader{vertexShaderFlat, fragmentShaderComposition}
{
    vertexBuffer.setBuffer<Point2D>("position", fullScreenQuad);
    compositionShader.activate();
    compositionShader.setVertexAttribute<Point2D>("position", vertexBuffer);
    compositionShader.deactivate();
}

void CompositionShader::draw(const GLTexture& color, const GLTexture& depth, const GLTexture& blur,
                             float minDepth, float maxDepth, float blurDepth)
{
    compositionShader.activate();
    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    color.bind();
    compositionShader.setUniform("colorTexture", 0);

    glActiveTexture(GL_TEXTURE1);
    depth.bind();
    // not a shadow map - we want the values
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    compositionShader.setUniform("depthTexture", 1);

    glActiveTexture(GL_TEXTURE2);
    blur.bind();
    compositionShader.setUniform("blurTexture", 2);

    compositionShader.setUniform("minDepth", minDepth);
    compositionShader.setUniform("maxDepth", maxDepth);
    compositionShader.setUniform("blurDepth", blurDepth);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glEnable(GL_DEPTH_TEST);
    compositionShader.deactivate();
}
