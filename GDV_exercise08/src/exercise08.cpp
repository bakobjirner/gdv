#include <common/constants.h>
#include <misc/image_processing.h>
#include <render/sampler.h>

#include <cmath>
#include <complex>
#include <iostream>
#include <stdexcept>

Texture DiscreteFourierTransform::dft(const Texture &input, bool inverse) {
  if (input.channels != Texture::Channels::RG ||
      input.dataType != Texture::DataType::Float) {
    throw std::runtime_error(
        "the input needs to be a 2-channel float texture.");
  }

  /// the result image
  Texture result{input.resolution, input.channels, input.dataType};

  std::span<const complex> inputPixels = input.getData<complex>();
  std::span<complex> resultPixels = result.getData<complex>();

  const uint32_t width = input.resolution.x;
  const uint32_t height = input.resolution.y;

  std::cout << "DFT running ..." << std::flush;
  /*
    for (uint32_t i = 0; i < inputPixels.size(); i++) {
      if (i % width == 0) {
        std::cout << std::endl;
      }
      std::cout << inputPixels[i];
    }
    */
  // make sure result is clear
  std::fill(resultPixels.begin(), resultPixels.end(), complex{0.0f, 0.0f});

  float sign = inverse ? 1 : -1;
  std::complex<float> imag = 0;
  imag.imag(1);

  // without optimization
  
    for (uint32_t u = 0; u < width; u++) {
      for (uint32_t v = 0; v < height; v++) {
        std::complex<float> sum = 0;
        for (uint32_t x = 0; x < width; x++) {
          std::complex<float> innerSum = 0;
          for (uint32_t y = 0; y < height; y++) {
            innerSum +=
                inputPixels[y * width + x] *
                exp(sign*imag * 2.0f * M_PIf * ((float)u / width) * (float)x) *
                exp(sign*imag * 2.0f * M_PIf * ((float)v / height) * (float)y);
          }
          sum += innerSum;
        }
        std::complex<float> fourierCoeffficient =
            (1 / sqrtf(width * height)) * sum;
        resultPixels[v * width + u] = fourierCoeffficient;
      }
    }
    



  std::cout << " done.\n" << std::flush;

  return result;
}

float SamplingPatterns::eval(Point2D pos) const {
  switch (function) {
    // TODO: implement the three functions given on the exercise sheet

  default: // checkerboard :) - not to be used
    return static_cast<float>(1 & (static_cast<int32_t>(pos.x * 2.0f) ^
                                   static_cast<int32_t>(pos.y * 2.0f)));
  }
}

std::vector<Point2D>
SamplingPatterns::generateSamplePosition(uint32_t numSamplesPerDim) const {
  std::vector<Point2D> result;
  result.reserve(numSamplesPerDim * numSamplesPerDim);

  // TODO: generate n = m^2 samples according to the given sampling strategies

  result.emplace_back(0.5f,
                      0.5f); // just sample the pixel center - replace this

  return result;
}

Texture SamplingPatterns::sample(Resolution res,
                                 uint32_t numSamplesPerDim) const {
  Texture result{res, Texture::Channels::R, Texture::DataType::Float};
  std::span<float> pixels = result.getData<float>();

  float invWidth = 1.0f / static_cast<float>(res.x);
  float invHeight = 1.0f / static_cast<float>(res.y);

#if defined(_WIN32)
  using OMPIndex = int32_t;
#else
  using OMPIndex = uint32_t;
#endif

#pragma omp parallel for
  for (OMPIndex y = 0; y < res.y; ++y) {
    for (uint32_t x = 0; x < res.x; ++x) {
      pixels[y * res.x + x] = 0.0f;
      if (numSamplesPerDim) {
        const auto samplePositions = generateSamplePosition(numSamplesPerDim);
        for (const Point2D &sample : samplePositions) {
          pixels[y * res.x + x] +=
              eval({(static_cast<float>(x) + sample.x) * invWidth,
                    (static_cast<float>(y) + sample.y) * invHeight});
        }
        pixels[y * res.x + x] *=
            1.0f / static_cast<float>(samplePositions.size());
      }
    }
  }

  return result;
}
