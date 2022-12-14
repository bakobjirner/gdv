#version 330 core

uniform mat4 mlp;
uniform mat4 mvp;
uniform mat4 model;

in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec4 vsLSPosition;
out vec3 vsWSPosition;
out vec3 vsWSNormal;
flat out vec3 vsWSNormalFlat;
out vec2 vsTexCoords;


void main() {
    gl_Position = mvp * vec4(position, 1.0);
    vsLSPosition = mlp * vec4(position, 1.0);
    vec4 wsPosition = model * vec4(position, 1.0);
    // the model matrix should not contain a projection, so 3D is enough
    vsWSPosition = wsPosition.xyz/wsPosition.w;
    vsWSNormal = transpose(inverse(mat3(model)))*normal;
    vsWSNormalFlat = vsWSNormal;
    vsTexCoords = texCoords;
}
