#version 410 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

uniform mat4 model;
uniform mat4 mvp;
uniform sampler2D displacementMap;
uniform float displacementScale;
uniform bool shadeFlat;
uniform bool textured;
uniform bool useDisplacementMapping;

in vec4 vsLSPosition[];
in vec4 vsWSPosition[];
in vec3 vsWSNormal[];
flat in vec3 vsWSNormalFlat[];
in vec2 vsTexCoords[];

out vec4 lsPosition;
out vec4 wsPosition;
out vec3 wsNormal;
flat out vec3 wsTangent;
flat out vec3 wsBitangent;
flat out vec3 wsNormalFlat;
out vec2 texCoords;

void main()
{
    // TODO: use vsWSPosition and vsTexCoords to compute the tangent and bi-tangent vectors
    float u1 = vsTexCoords[0].x;
    float u2 = vsTexCoords[1].x;
    float u3 = vsTexCoords[2].x;
    float v1 = vsTexCoords[0].y;
    float v2 = vsTexCoords[1].y;
    float v3 = vsTexCoords[2].y;

    vec3 x1 = vsWSPosition[0].xyz;
    vec3 x2 = vsWSPosition[1].xyz;
    vec3 x3 = vsWSPosition[2].xyz;


    float denominator = (u2 - u1 ) * (v3 - v1 ) - (v2 - v1 ) * (u3 - u1 );
    vec3 s = ((x2 - x1 ) * (v3 - v1 ) - (x3 - x1 ) * (v2 - v1 ))/denominator;
    vec3 t = ((x3 - x1 ) * (u2 - u1 ) - (x2 - x1 ) * (u3 - u1 ))/denominator;
    s = normalize(s);
    t = normalize(t);

    // now, compute the per-vertex data

    for (int vertex = 0; vertex < 3; vertex++)
    {
        lsPosition    = vsLSPosition[vertex];
        wsPosition    = vsWSPosition[vertex];
        wsNormal      = vsWSNormal[vertex];
        wsTangent     = s;
        wsBitangent   = t;
        wsNormalFlat  = vsWSNormalFlat[vertex];
        texCoords = vsTexCoords[vertex];
        gl_Position = gl_in[vertex].gl_Position;
        if (textured && useDisplacementMapping) {
            vec3 normal = shadeFlat ? wsNormalFlat : wsNormal;
            mat4 vp = mvp * inverse(model);
            float displacement= texture(displacementMap, texCoords).x;
            //rescale
            displacement = (displacement - 0.5)*displacementScale;
            //multiply with normal
            vec3 displacementVector = displacement * normal;
            wsPosition = wsPosition + vec4(displacementVector,0);
            gl_Position = vp*wsPosition;
            // TODO: overwrite the gl_Position with the displaced position
            // when applying the model view projection matrix, make sure to set w=1 in your vec4
        }

        EmitVertex();
    }

    EndPrimitive();
}
