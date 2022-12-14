#include <render/gl/gl_utils.h>

#include <nanogui/opengl.h>

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

using namespace std::string_literals;

void checkGL(const char* file, const int line)
{
    const GLenum error = glGetError();

    if (error != GL_NO_ERROR) {
        std::ostringstream message;
        message << "OpenGL error 0x" << std::hex << error << " in file " << file << ", line "
                << std::dec << line;
        throw std::runtime_error(message.str());
    }
}

std::string readShaderFile(const std::string_view filename)
{
    std::ifstream file{filename.data(), std::ifstream::in};
    std::stringstream buffer;
    buffer << file.rdbuf();
    if (!file)
        throw std::runtime_error("failed to read shader file "s + std::string(filename));
    file.close();
    return buffer.str();
}

GLuint GLShaderProgram::compileShaderProgram(const std::string_view vertexShader,
                                             const std::string_view fragmentShader,
                                             const std::string_view geometryShader,
                                             const std::string_view tessControlShader,
                                             const std::string_view tessEvalShader)
{
    const std::array<GLenum, 5> shaderTypes = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER,
                                               GL_GEOMETRY_SHADER, GL_TESS_CONTROL_SHADER,
                                               GL_TESS_EVALUATION_SHADER};
    const std::array<std::string_view, shaderTypes.size()> shaders = {
        vertexShader, fragmentShader, geometryShader, tessControlShader, tessEvalShader};

    GLuint program = glCreateProgram();
    for (uint32_t i = 0; i < shaders.size(); ++i) {
        if (shaders[i].empty())
            continue;

        const GLuint shader = glCreateShader(shaderTypes[i]);
        const GLint shaderSize = static_cast<GLint>(shaders[i].size());
        const GLchar* const shaderSource = shaders[i].data();
        glShaderSource(shader, 1, &shaderSource, &shaderSize);
        glCompileShader(shader);
        CHECK_GL();
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        if (length) {
            GLchar* buffer = new char[static_cast<size_t>(length)];
            glGetShaderInfoLog(shader, length, nullptr, buffer);
            std::cerr << "OpenGL shader compile log:\n" << buffer << "\n";
            delete[] buffer;
        }

        glAttachShader(program, shader);
        // delete the shader as soon as no program references it anymore
        glDeleteShader(shader);
        CHECK_GL();
    }
    glLinkProgram(program);
    CHECK_GL();
    // if something went wrong, print an error message
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    if (length) {
        GLchar* buffer = new char[static_cast<size_t>(length)];
        glGetProgramInfoLog(program, length, nullptr, buffer);
        std::cerr << "OpenGL shader link log:\n" << buffer << "\n";
        delete[] buffer;
    }
    CHECK_GL();

    return program;
}

void GLVertexBuffer::draw(const std::string_view name, GLenum mode, size_t from, size_t to) const
{
    if (to == from)
        return;
    if (to < from)
        throw std::range_error("invalid index range");

    uint32_t elementSize;
    switch (mode) {
    case GL_POINTS:
        elementSize = 1;
        break;
    case GL_LINES:
        elementSize = 2;
        break;
    case GL_PATCHES:
        // assume we're using triangles for tesselation
    case GL_TRIANGLES:
        elementSize = 3;
        break;
    default:
        throw std::logic_error("unhandled drawing mode");
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer(name));
    const GLsizei size = static_cast<GLsizei>((to - from) * elementSize);
    const GLvoid* start = reinterpret_cast<const GLvoid*>(from * sizeof(uint32_t) * elementSize);
    glDrawElements(mode, size, GL_UNSIGNED_INT, start);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
