#version 330 core

uniform vec3 albedo;
uniform bool shadeFlat;
uniform vec3 cameraPos;
uniform vec3 pointLightPos;
uniform vec3 pointLightPower;

uniform sampler2D albedoTexture;
uniform sampler2D normalMap;
uniform sampler2D roughnessTexture;
uniform sampler2D metalTexture;
uniform sampler2D envmap;
uniform sampler2D brdfLUT;
uniform sampler2DShadow shadowMap;

in vec4 lsPosition;
in vec3 wsPosition;
in vec3 wsNormal;
in vec3 wsTangent;
in vec3 wsBitangent;
flat in vec3 wsNormalFlat;
in vec2 texCoords;
out vec3 color;

const float pi = 3.14159265359f;
const float epsilon = 0.001f;

uniform bool textured;
uniform bool useNormalMapping;
uniform bool useShadowMapping;
uniform bool useMetalTexture;
uniform int envmapMaxLevel;

uniform float alpha;
uniform vec3 F0;

/// microfacet distribution
float D(float cosThetaM, float alpha) {
    if (cosThetaM <= 0.0f)
        return 0.0f;
    float cosThetaMSqr = cosThetaM*cosThetaM;
    float alphaSqr = alpha*alpha;
    float x = 1.0f+cosThetaMSqr*(alphaSqr-1.0f);
    return alphaSqr/(pi*x*x);
}
float G1(float cosThetaV, float alpha) {
    float cosThetaVSqr = cosThetaV*cosThetaV;
    float sinThetaVSqr = 1.0f-cosThetaVSqr;
    float tanThetaVSqr = sinThetaVSqr/cosThetaVSqr;
    return 2.0f/(1.0f+sqrt(1.0f+alpha*alpha*tanThetaVSqr));
}
/// shadowing-masking term
float G(float cosThetaI, float cosThetaO, float alpha) {
    return G1(cosThetaI, alpha)*G1(cosThetaO, alpha);
}

void main() {
    color = vec3(0.0f);

    if (gl_FrontFacing) {
        vec3 wsPos = wsPosition;
        vec3 omegaO = normalize(cameraPos-wsPos);
        vec3 normal = normalize(shadeFlat ? wsNormalFlat : wsNormal);
        // backface test
        if (dot(omegaO, normal) <= 0.0f)
            return;

        if (textured && useNormalMapping) {

            vec3 s = normalize(wsTangent);
            vec3 t = normalize(wsBitangent);

            normal = normalize(mat3(s,t,normal)*(texture(normalMap, texCoords).rgb*2.0f-1.0f));
        }

        float cosThetaO = dot(omegaO, normal);

        // diffuse shading
        vec3 diffuseAlbedo = textured ? texture(albedoTexture, texCoords).rgb : albedo;

        // "metalness" parameter - interpolates between dielectric + diffuse and conductor
        float metal = 0.0f;
        float roughness = alpha;
        if (textured) {
            if (useMetalTexture)
                metal = texture(metalTexture, texCoords).r;
            roughness = texture(roughnessTexture, texCoords).r;
        }

        if (envmapMaxLevel > 0) {
            // illumination by pre-filtered environment map

            vec3 LiDiffuse = vec3(0.0f);
            vec3 LiSpecular = vec3(0.0f);

            // diffuse
            {
                float theta = acos(normal.y);
                float phi = atan(-normal.x, normal.z);
                vec3 irradiance = textureLod(envmap, vec2(phi/(2.0f*pi)+0.5f, theta/pi), envmapMaxLevel).rgb;
                LiDiffuse = diffuseAlbedo*irradiance;
            }

            // specular
            {
                vec3 omegaI = normal*(2.0f*cosThetaO)-omegaO;
                float theta = acos(omegaI.y);
                float phi = atan(-omegaI.x, omegaI.z);

                // we define the mip-levels and textures in terms of alphaSqr
                // (this gives us more detail for low roughness values)
                // so we need to take the square-root here to cancel this out
                float alphaSqrt = sqrt(roughness);

                vec3 prefilteredEnvmap = textureLod(envmap, vec2(phi/(2.0f*pi)+0.5f, theta/pi), alphaSqrt*envmapMaxLevel).rgb;
                vec2 prefilteredFresnel = texture(brdfLUT, vec2(cosThetaO, alphaSqrt)).rg;

                LiSpecular = prefilteredEnvmap*(mix(F0, diffuseAlbedo, metal)*prefilteredFresnel.x + prefilteredFresnel.y);
            }

            color += LiDiffuse*(1.0f-metal)+LiSpecular;
        }

        {
            // illumination by point light

            vec3 LiDiffuse = vec3(0.0f);
            vec3 LiSpecular = vec3(0.0f);

            vec3 wsDir = pointLightPos-wsPos;
            float dist = length(wsDir);
            vec3 omegaI = normalize(wsDir);
            float cosThetaI = dot(normal, omegaI);

            // illumination on the front side only
            if (cosThetaI <= 0.0f || cosThetaO <= 0.0f)
                return;

            bool shaded = false;

            if (useShadowMapping) {
                // shadow map
                vec3 lsTexCoord = lsPosition.xyz/lsPosition.w*0.5+0.5;
                lsTexCoord.z *= (1.0f-epsilon);

                if (lsTexCoord.z <= 1.0f) {
                    float shadow = texture(shadowMap, lsTexCoord);
                    shaded = (shadow <= 0.0f);
                }
            }

            if (shaded)
                return;

            LiDiffuse = diffuseAlbedo*cosThetaI/pi;

            if (alpha > 0.0f)
            {
                vec3 halfway = normalize(omegaI+omegaO);
                float cosThetaM = dot(normal, halfway);
                float cosThetaIM = dot(halfway, omegaI);
                if (cosThetaIM > 0.0f) {
                    float u = 1.0f-cosThetaIM;
                    float u2 = u*u;
                    float schlickFresnel = u2*u2*u;
                    vec3 fresnel = mix(mix(F0, diffuseAlbedo, metal), vec3(1.0f), schlickFresnel);
                    float shadowing = G(cosThetaI, cosThetaO, roughness);
                    float microfacet = D(cosThetaM, roughness);

                    // cosThetaI cancels out
                    LiSpecular = fresnel*(shadowing*microfacet/(4.0f*cosThetaO));
                }
            }

            // multiply with point light emission
            color += (LiDiffuse*(1.0f-metal)+LiSpecular) * pointLightPower/(dist*dist*(4.0f*pi));
        }
    }
}
