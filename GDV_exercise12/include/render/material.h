#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "texture.h"
#include <common/constants.h>
#include <geometry/frame.h>
#include <geometry/point3d.h>

#include <string>
#include <variant>

struct Material {
    static Vector3D reflect(Vector3D v) { return {-v.x, -v.y, v.z}; }

    // surface material models

    struct Null {};
    struct Diffuse {
        Color albedo;

        Color eval(float cosThetaI) const
        {
            if (cosThetaI > 0.0f)
                return albedo * invPi;

            return {0.0f};
        }
    };
    struct Mirror {};
    struct Conductor {
        Color eta;
        Color k;

        Color fresnel(float cosTheta) const
        {
            const float cosThetaSqr = cosTheta * cosTheta;
            const float sinThetaSqr = 1.0f - cosThetaSqr;
            const Color etaSqr = eta * eta;
            const Color kSqr = k * k;

            const Color temp1 = etaSqr - kSqr - sinThetaSqr;
            const Color aSqrPlusBSqr = sqrt(temp1 * temp1 + etaSqr * kSqr * 4.0f);
            const Color aSqr = (aSqrPlusBSqr + temp1) * 0.5f;

            const Color twoACosTheta = sqrt(aSqr) * (2.0f * cosTheta);
            const Color Rs = (aSqrPlusBSqr + cosThetaSqr - twoACosTheta)
                           / (aSqrPlusBSqr + cosThetaSqr + twoACosTheta);

            const Color temp2 = twoACosTheta * sinThetaSqr;
            // the fraction is multiplied by cosThetaSqr/cosThetaSqr to avoid computing tanThetaSqr
            const Color Rp = Rs
                           * ((aSqrPlusBSqr * cosThetaSqr + sinThetaSqr * sinThetaSqr - temp2)
                              / (aSqrPlusBSqr * cosThetaSqr + sinThetaSqr * sinThetaSqr + temp2));

            return (Rs + Rp) * 0.5f;
        }
    };
    struct Dielectric {
        /// internal index of refraction
        float etaI;
        /// external index of refraction
        float etaO;

        Vector3D refract(Vector3D omegaO) const
        {
            const float etaT{omegaO.z >= 0.0f ? this->etaI : this->etaO};
            const float etaO{omegaO.z >= 0.0f ? this->etaO : this->etaI};

            const float eta = etaO / etaT;
            const float x = -omegaO.x * eta;
            const float y = -omegaO.y * eta;
            const float zSqr = 1.0f - x * x - y * y;

            if (zSqr <= 0.0f) // total internal reflection
                return {};

            const float z = std::sqrt(zSqr);
            return {x, y, (omegaO.z >= 0.0f) ? -z : z};
        }
        float fresnel(float cosThetaO, float cosThetaT) const
        {
            if (cosThetaT == 0.0f) // total internal reflection
                return 1.0f;

            const float etaO{cosThetaO >= 0.0f ? this->etaO : this->etaI};
            const float etaT{cosThetaO >= 0.0f ? this->etaI : this->etaO};

            cosThetaO = std::abs(cosThetaO);
            cosThetaT = std::abs(cosThetaT);

            const float Rs =
                (etaO * cosThetaO - etaT * cosThetaT) / (etaO * cosThetaO + etaT * cosThetaT);
            const float Rp =
                (etaT * cosThetaO - etaO * cosThetaT) / (etaT * cosThetaO + etaO * cosThetaT);

            return (Rs * Rs + Rp * Rp) * 0.5f;
        }
    };
    struct GGX {
        /// roughness (0 = perfectly specular, 1 = diffuse)
        float alpha;

        /// microfacet distribution
        float D(float cosThetaM) const
        {
            if (cosThetaM <= 0.0f)
                return 0.0f;
            const float cosThetaMSqr = cosThetaM * cosThetaM;
            const float alphaSqr = alpha * alpha;
            // tanThetaMSqr nicely cancels out
            // const float x = cosThetaMSqr*(alphaSqr+tanThetaMSqr);
            const float x = 1.0f + cosThetaMSqr * (alphaSqr - 1.0f);
            return alphaSqr / (pi * x * x);
        }
        float G1(float cosThetaV) const
        {
            const float cosThetaVSqr = cosThetaV * cosThetaV;
            const float sinThetaVSqr = 1.0f - cosThetaVSqr;
            const float tanThetaVSqr = sinThetaVSqr / cosThetaVSqr;
            return 2.0f / (1.0f + std::sqrt(1.0f + alpha * alpha * tanThetaVSqr));
        }
        /// shadowing-masking term
        float G(float cosThetaI, float cosThetaO) const { return G1(cosThetaI) * G1(cosThetaO); }
    };
    struct RoughConductor {
        Conductor conductor{};
        GGX microfacetDistribution{};

        Color eval(Vector3D omegaO, Vector3D omegaI) const
        {
            const float cosThetaI = omegaI.z;
            const float cosThetaO = omegaO.z;
            if (cosThetaI * cosThetaO <= 0.0f)
                return {0.0f};

            const Normal3D halfway = normalize(omegaO + omegaI);
            const float cosThetaM = halfway.z;
            const float cosThetaIM = dot(halfway, omegaI);
            if (cosThetaIM <= 0.0f) // technically part of G1
                return {0.0f};

            const Color F = conductor.fresnel(cosThetaIM);
            const float G = microfacetDistribution.G(cosThetaI, cosThetaO);
            const float D = microfacetDistribution.D(cosThetaM);

            return F * (G * D / (4.0f * cosThetaI * cosThetaO));
        }
    };
    struct RoughPlastic {
        Diffuse diffuse{};
        Dielectric dielectric{};
        GGX microfacetDistribution{};

        Color eval(Vector3D omegaO, Vector3D omegaI) const
        {
            const float cosThetaI = omegaI.z;
            const float cosThetaO = omegaO.z;
            if (cosThetaI * cosThetaO <= 0.0f)
                return {0.0f};

            const Normal3D halfway = normalize(omegaO + omegaI);
            const float cosThetaM = halfway.z;
            const float cosThetaIM = dot(halfway, omegaI);
            if (cosThetaIM <= 0.0f) // technically part of G1
                return {0.0f};

            // reflection only - so we can reuse cosThetaIM here
            const float F = dielectric.fresnel(cosThetaIM, cosThetaIM);
            const float G = microfacetDistribution.G(cosThetaI, cosThetaO);
            const float D = microfacetDistribution.D(cosThetaM);

            return diffuse.eval(cosThetaI) * (1.0f - F)
                 + F * (G * D / (4.0f * cosThetaI * cosThetaO));
        }
    };

    // volume material models

    struct Vacuum {};
    struct Absorption {
        /// fraction of light absorbed per unit distance traveled in the medium
        Color absorptionCoefficient;

        Color transmittance(float distance) const { return exp(absorptionCoefficient * -distance); }
    };

    using SurfaceMaterial =
        std::variant<Null, Diffuse, Mirror, Conductor, Dielectric, RoughConductor, RoughPlastic>;
    using VolumeMedium = std::variant<Vacuum, Absorption>;

    SurfaceMaterial surface{};
    struct Textures {
        Texture albedo{};
        Texture normal{};
        Texture roughness{};
        Texture metal{};
        Texture displacement{};
        float displacementScale{0.125f};
    } textures{};

    Color emittedRadiance{};

    VolumeMedium medium{};

    // check what type of material this is

    bool isNull() const { return std::holds_alternative<Null>(surface); }
    bool isDiffuse() const { return std::holds_alternative<Diffuse>(surface); }
    bool isMirror() const { return std::holds_alternative<Mirror>(surface); }
    bool isDielectric() const { return std::holds_alternative<Dielectric>(surface); }
    bool isConductor() const { return std::holds_alternative<Conductor>(surface); }
    bool isRoughConductor() const { return std::holds_alternative<RoughConductor>(surface); }
    bool isRoughPlastic() const { return std::holds_alternative<RoughPlastic>(surface); }

    bool isTransmissive() const { return isNull() || isDielectric(); }
    bool isRough() const { return isDiffuse() || isRoughConductor() || isRoughPlastic(); }

    bool isEmitter() const { return !emittedRadiance.isBlack(); }

    bool isTextured() const
    {
        return textures.albedo || textures.normal || textures.roughness || textures.displacement;
    }

    bool isVacuum() const { return std::holds_alternative<Vacuum>(medium); }
    bool isAbsorption() const { return std::holds_alternative<Absorption>(medium); }
    bool containsMedium() const { return isAbsorption(); }

    // get a specific material (make sure it is of that type first!)

    Diffuse diffuse() const { return std::get<Diffuse>(surface); }
    Mirror mirror() const { return std::get<Mirror>(surface); }
    Dielectric dielectric() const { return std::get<Dielectric>(surface); }
    Conductor conductor() const { return std::get<Conductor>(surface); }
    RoughConductor roughConductor() const { return std::get<RoughConductor>(surface); }
    RoughPlastic roughPlastic() const { return std::get<RoughPlastic>(surface); }
    Absorption absorption() const { return std::get<Absorption>(medium); }

    Color eval(Vector3D omegaO, Vector3D omegaI) const
    {
        if (isDiffuse()) {
            return diffuse().eval(omegaI.z);
        }
        else if (isRoughConductor()) {
            return roughConductor().eval(omegaO, omegaI);
        }
        else if (isRoughPlastic()) {
            return roughPlastic().eval(omegaO, omegaI);
        }
        return {0.0f};
    }

    /// get some color representing this material
    Color albedo() const
    {
        if (isDiffuse())
            return diffuse().albedo;
        else if (isConductor())
            return conductor().fresnel(1.0f);
        else if (isDielectric())
            return dielectric().fresnel(1.0f, 1.0f);
        else if (isRoughConductor())
            return roughConductor().eval({0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f});
        else if (isRoughPlastic())
            return roughPlastic().eval({0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f});
        else
            return Color{165 / 255.0f, 30 / 255.0f, 55 / 255.0f, 1.0f};
    }
};

#endif // MATERIAL_H