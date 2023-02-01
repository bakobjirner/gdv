#ifndef POST_PROCESSING_SHADER_H
#define POST_PROCESSING_SHADER_H

#include <geometry/matrix4d.h>
#include "gl_utils.h"

class BlurShader final {
public:
    BlurShader();
    void draw(const GLTexture& color, bool horizontal);

private:
    GLShaderProgram blurShader;
    GLVertexBuffer vertexBuffer{};

    static const std::string fragmentShaderBlur;
};

class CompositionShader final {
public:
    CompositionShader();
    void draw(const GLTexture& color, const GLTexture& depth, const GLTexture& blur,
              float minDepth, float maxDepth, float blurDepth);

private:
    GLShaderProgram compositionShader;
    GLVertexBuffer vertexBuffer{};

    static const std::string fragmentShaderComposition;
};

#endif // POST_PROCESSING_SHADER_H
