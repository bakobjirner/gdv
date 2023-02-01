#version 330 core

uniform mat4 mlp;
uniform mat4 mvp;
uniform mat4 model;

in vec3 position;
in vec3 normal;

out vec4 lsPosition;
out vec3 wsPosition;
out vec3 wsNormal;
out vec3 wsTangent;
out vec3 wsBitangent;
flat out vec3 wsNormalFlat;
out vec2 texCoords;

/**
  * Simplified vertex shader to work wihout a geometry shader.
  * (for systems that have troubles running a geometry shader, i.e. Mac.)
  */
void main() {
    gl_Position = mvp * vec4(position, 1.0);
    lsPosition = mlp * vec4(position, 1.0);
    vec4 wsPosition4D = model * vec4(position, 1.0);
    // the model matrix should not contain a projection, so 3D is enough
    wsPosition = wsPosition4D.xyz/wsPosition4D.w;
    wsNormal = transpose(inverse(mat3(model)))*normal;
    wsNormalFlat = wsNormal;

    // these values are not needed for simple shading - set some constant defaults
    wsTangent = vec3(0.0);
    wsBitangent = vec3(0.0);
    texCoords = vec2(0.5);
}
