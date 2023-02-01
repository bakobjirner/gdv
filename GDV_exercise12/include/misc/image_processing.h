#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <complex>
#include <geometry/point2d.h>
#include <map>
#include <render/color.h>
#include <render/texture.h>
#include <span>
#include <vector>

class DiscreteFourierTransform {
public:
    using complex = std::complex<float>;

    /// (inverse) discrete fourier transform for a two-channel float texture
    static Texture dft(const Texture& input, bool inverse = false);
};

class SamplingPatterns {
public:
    enum class Function { CampbellRobson, SmoothZonePlate, SiemensStar };
    enum class SamplingStrategy { Regular, Random, Stratified };

    Function function{Function::CampbellRobson};
    SamplingStrategy samplingStrategy{SamplingStrategy::Regular};

    /// evaluate one of the test-functions for anti-aliasing
    float eval(Point2D pos) const;

    /**
     * @brief generate sub-pixel sample positions
     * @param numSamplesPerDim number of samples per x/y-dimension
     * @return numSamplesPerDim*numSamplesPerDim many samples
     */
    std::vector<Point2D> generateSamplePosition(uint32_t numSamplesPerDim) const;

    /// sample the function into a texture of the given resolution
    /// evaluate each pixel's value with the given number of samples per x/y-dimension
    Texture sample(Resolution res, uint32_t numSamplesPerDim) const;
};

class SpectralToRGB {
public:
    void loadWavelengthToXYZ(const std::string_view filename);
    void loadWavelengthToD65(const std::string_view filename);

    void loadTextures(const std::vector<std::pair<uint32_t, Texture>>& textures)
    {
        this->textures = textures;
    }
    void loadTextures(std::vector<std::pair<uint32_t, Texture>>&& textures)
    {
        this->textures = std::move(textures);
    }
    Texture computeXYZTexture(Pixel referencePoint) const;

    static Vector3D xyzToLinearRGB(Vector3D c);
    static Vector3D linearRGBToSRGB(Vector3D c);

    static Texture xyzToSRGB(const Texture& xyzTexture);

private:
    std::map<uint32_t, float> wavelengthToD65;
    std::map<uint32_t, Vector3D> wavelengthToXYZ;
    std::vector<std::pair<uint32_t, Texture>> textures;
};

#endif // IMAGE_PROCESSING_H
