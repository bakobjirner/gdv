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
in vec3 vsWSPosition[];
in vec3 vsWSNormal[];
flat in vec3 vsWSNormalFlat[];
in vec2 vsTexCoords[];

out vec4 lsPosition;
out vec3 wsPosition;
out vec3 wsNormal;
out vec3 wsTangent;
out vec3 wsBitangent;
flat out vec3 wsNormalFlat;
out vec2 texCoords;

void main()
{
    // compute the tangent space once for the entire triangle

    vec2 uv12 = vsTexCoords[1]-vsTexCoords[0];
    vec2 uv13 = vsTexCoords[2]-vsTexCoords[0];
    vec3 v1v2 = vsWSPosition[1].xyz-vsWSPosition[0].xyz;
    vec3 v1v3 = vsWSPosition[2].xyz-vsWSPosition[0].xyz;

    /*
    // we know this needs to hold for our tangent frame
    v1v2 == uv12.x*s+uv12.y*t;
    v1v3 == uv13.x*s+uv13.y*t;

    // now, solve for s and t
    s = (v1v2-t*uv12.y)/uv12.x;
    t = (v1v3-s*uv13.x)/uv13.y;

    s = v1v2/uv12.x-t*uv12.y/uv12.x;
    t = v1v3/uv13.y-s*uv13.x/uv13.y;

    // insert s in t
    t = (v1v3*uv12.x-v1v2*uv13.x)/(uv12.x*uv13.y) + t*uv12.y*uv13.x/(uv12.x*uv13.y);

    // solve for t
    t*(uv12.x*uv13.y)/(uv12.y*uv13.x) = (v1v3*uv12.x-v1v2*uv13.x)/(uv12.y*uv13.x) + t;
    t*(uv12.x*uv13.y-uv12.y*uv13.x)/(uv12.y*uv13.x) = (v1v3*uv12.x-v1v2*uv13.x)/(uv12.y*uv13.x);
    t*(uv12.x*uv13.y-uv12.y*uv13.x) = (v1v3*uv12.x-v1v2*uv13.x);
    t = (v1v3*uv12.x-v1v2*uv13.x)/(uv12.x*uv13.y-uv12.y*uv13.x);

    // insert t in s (optional)
    s = v1v2/uv12.x-(v1v3*uv12.x-v1v2*uv13.x)/(uv12.x*uv13.y-uv12.y*uv13.x)*uv12.y/uv12.x;
    s*uv12.x = v1v2-(v1v3*uv12.x*uv12.y-v1v2*uv13.x*uv12.y)/(uv12.x*uv13.y-uv12.y*uv13.x);
    s*uv12.x = (v1v2*(uv12.x*uv13.y-uv12.y*uv13.x)-v1v2*uv13.x*uv12.y-v1v3*uv12.x*uv12.y)/(uv12.x*uv13.y-uv12.y*uv13.x);
    s*uv12.x = (v1v2*uv12.x*uv13.y-v1v3*uv12.x*uv12.y)/(uv12.x*uv13.y-uv12.y*uv13.x);
    s = (v1v2*uv13.y-v1v3*uv12.y)/(uv12.x*uv13.y-uv12.y*uv13.x);
    */

    // do the computation
    float div = 1.0f/(uv12.x*uv13.y-uv12.y*uv13.x);
    vec3 s = (v1v2*uv13.y-v1v3*uv12.y)*div;
    vec3 t = (v1v3*uv12.x-v1v2*uv13.x)*div;

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

            // we ignore the existing position and just compute the displacement in world space
            float displacement = displacementScale*(texture(displacementMap, texCoords).r-0.5f);
            gl_Position = vp * vec4(wsPosition + normal*displacement, 1.0f);
        }

        EmitVertex();
    }

    EndPrimitive();
}
