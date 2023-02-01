#include <misc/image_processing.h>
#include <render/gl/post_processing_shader.h>

#include <geometry/matrix3d.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std::string_literals;

const std::string BlurShader::fragmentShaderBlur = R"(
#version 330

uniform sampler2D colorTexture;
uniform bool horizontal; // whether to blur horizontally or vertically

out vec3 color;

void main() {
    ivec2 texSize = textureSize(colorTexture, 0);
    vec2 pixelSize = vec2(1.0)/texSize; // assuming the screen to be as big as the input texture
    vec2 texCoord = gl_FragCoord.xy*pixelSize;

    color = texture(colorTexture, texCoord).rgb;

    // TODO: lookup the horizontally/vertically neighboring pixels
    // and add their weighted contribution to the output color
}
)"s;

void SpectralToRGB::loadWavelengthToXYZ(const std::string_view filename)
{
    wavelengthToXYZ.clear();

    std::ifstream csvFile{filename.data(), std::ios_base::in};

    // TODO: read in the CSV file
    std::string data;
    while(csvFile){
    csvFile >> data;
    std::cout<<data<<std::endl;
    }
   

    std::cout << "Read wavelength to XYZ file with " << wavelengthToXYZ.size() << " entries."
              << std::endl;
}

void SpectralToRGB::loadWavelengthToD65(const std::string_view filename)
{
    wavelengthToD65.clear();

    std::ifstream csvFile{filename.data(), std::ios_base::in};

    // TODO: read in the CSV file

    std::cout << "Read wavelength to D65 file with " << wavelengthToD65.size() << " entries."
              << std::endl;
}

Texture SpectralToRGB::computeXYZTexture(Pixel referencePoint) const
{
    if (wavelengthToXYZ.empty())
        throw std::logic_error("Load the wavelength to XYZ file first");

    Texture result{};

    for (size_t i = 0; i < textures.size(); ++i) {
        const uint32_t wavelength = textures.at(i).first;
        const Texture& input = textures.at(i).second;

        if (i == 0) {
            result = Texture{input.resolution, Texture::Channels::RGB, Texture::DataType::Float};
            std::span<Vector3D> outputPixels = result.getData<Vector3D>();
            for (size_t pixel = 0; pixel < result.resolution.x * result.resolution.y; ++pixel)
                outputPixels[pixel] = {};
        }
        else if (input.resolution != result.resolution)
            throw std::runtime_error("All textures need to have the same resolution.");
        if (input.channels != Texture::Channels::R || input.dataType != Texture::DataType::Float)
            throw std::runtime_error("Expected monochromatic float input.");

        const std::span<const float> inputPixels = input.getData<float>();
        std::span<Vector3D> outputPixels = result.getData<Vector3D>();

        float scale = 0.0f;
        if (referencePoint != Pixel{}) {
            if (wavelengthToD65.empty())
                throw std::logic_error("Load the wavelength to D65 file first");

            // normalization of 0.5 grey color to D65 illuminant
            referencePoint.x = std::clamp(referencePoint.x, 4U, input.resolution.x - 5U);
            referencePoint.y = std::clamp(referencePoint.y, 4U, input.resolution.y - 5U);
            for (uint32_t y = referencePoint.y - 4; y <= referencePoint.y + 4; ++y)
                for (uint32_t x = referencePoint.x - 4; x <= referencePoint.x + 4; ++x)
                    scale += inputPixels[y * input.resolution.x + x];
            // normalize input to D65 illumination * 0.5
            // divide by D65 reference of 100
            // divide by number of pixels averaged over (25)
            // and divide by number of input images
            scale = wavelengthToD65.at(wavelength)
                  / (scale * static_cast<float>(textures.size()) * (50.0f / 25.0f));
        }
        else {
            scale = 1.0f / static_cast<float>(textures.size());
        }

        // TODO: sum up the input pixels into the result,
        // scaled by the XYZ response for the given wavelength
    }

    return result;
}

Vector3D SpectralToRGB::xyzToLinearRGB(Vector3D c)
{
    // TODO: convert XYZ to RGB
    return c; // FIXME
}

Vector3D SpectralToRGB::linearRGBToSRGB(Vector3D c)
{
    // TODO: convert linear RGB to sRGB
    return c; // FIXME
}

Texture SpectralToRGB::xyzToSRGB(const Texture& xyzTexture)
{
    if (xyzTexture.channels != Texture::Channels::RGB
        || xyzTexture.dataType != Texture::DataType::Float)
        throw std::runtime_error("Expected RGB float texture.");

    Texture result{xyzTexture.resolution, Texture::Channels::RGB, Texture::DataType::Float};

    const std::span<const Vector3D> inputPixels = xyzTexture.getData<Vector3D>();
    std::span<Vector3D> outputPixels = result.getData<Vector3D>();

    for (size_t pixel = 0; pixel < result.resolution.x * result.resolution.y; ++pixel) {
        outputPixels[pixel] = linearRGBToSRGB(xyzToLinearRGB(inputPixels[pixel]));
    }

    return result;
}
