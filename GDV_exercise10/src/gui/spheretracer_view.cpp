#include <gui/spheretracer_view.h>

#include <string>

using namespace std::literals::string_literals;

static const std::string vertex_shader_imageview = R"(
#version 330

uniform mat4 matrix_image;
uniform mat4 matrix_background;
in vec2 position;
out vec2 position_background;
out vec2 uv;

void main() {
    vec4 p = vec4(position, 0.0, 1.0);
    gl_Position = matrix_image * p;
    position_background = (matrix_background * p).xy;
    uv = position;
}
)"s;

static const std::string fragment_shader_imageview = R"(
#version 330

in vec2 uv;
in vec2 position_background;
out vec4 frag_color;
uniform sampler2D image;
uniform vec4 background_color;
uniform float blur;
uniform bool useSRGB;
uniform bool falsecolor;

// BEGIN TuroboColormap
// Copyright 2019 Google LLC.
// SPDX-License-Identifier: Apache-2.0

// Polynomial approximation in GLSL for the Turbo colormap
// Original LUT: https://gist.github.com/mikhailov-work/ee72ba4191942acecc03fe6da94fc73f

// Authors:
//   Colormap Design: Anton Mikhailov (mikhailov@google.com)
//   GLSL Approximation: Ruofei Du (ruofei@google.com)

vec3 TurboColormap(in float x) {
  const vec4 kRedVec4 = vec4(0.13572138, 4.61539260, -42.66032258, 132.13108234);
  const vec4 kGreenVec4 = vec4(0.09140261, 2.19418839, 4.84296658, -14.18503333);
  const vec4 kBlueVec4 = vec4(0.10667330, 12.64194608, -60.58204836, 110.36276771);
  const vec2 kRedVec2 = vec2(-152.94239396, 59.28637943);
  const vec2 kGreenVec2 = vec2(4.27729857, 2.82956604);
  const vec2 kBlueVec2 = vec2(-89.90310912, 27.34824973);

  x = min(1.0f, x);
  vec4 v4 = vec4( 1.0, x, x * x, x * x * x);
  vec2 v2 = v4.zw * v4.z;
  return vec3(
    dot(v4, kRedVec4)   + dot(v2, kRedVec2),
    dot(v4, kGreenVec4) + dot(v2, kGreenVec2),
    dot(v4, kBlueVec4)  + dot(v2, kBlueVec2)
  );
}
// END TuroboColormap

void main() {
    vec2 frac = position_background - floor(position_background);
    float checkerboard = ((frac.x > .5) == (frac.y > .5)) ? 0.4 : 0.5;

    vec4 background = (1.0 - background_color.a) * vec4(vec3(checkerboard), 1.0) +
                              background_color.a * vec4(background_color.rgb, 1.0);

    vec4 value = texture(image, uv, blur);

    if (useSRGB) {
        vec3 srgb = vec3(
            value.r < 0.0031308 ? 12.92 * value.r : (1.055 * pow(value.r, 1.0/2.4) - 0.055),
            value.g < 0.0031308 ? 12.92 * value.g : (1.055 * pow(value.g, 1.0/2.4) - 0.055),
            value.b < 0.0031308 ? 12.92 * value.b : (1.055 * pow(value.b, 1.0/2.4) - 0.055)
        );

        frag_color = (1.0 - value.a) * background + vec4(srgb, value.a);
    }
    else if (falsecolor) {
        frag_color = (1.0 - value.a) * background + vec4(TurboColormap(log2(1.0f+value.r/256.0f)), value.a);
    }
    else
        frag_color = (1.0 - value.a) * background + value;
}
)"s;

SphereTracerView::SphereTracerView(nanogui::Widget* parent, Toolbar* toolbar,
                                   SphereTracer& sphereTracer,
                                   nanogui::ref<CameraControls> cameraControls)
    : ImageView(parent), sphereTracer{sphereTracer}, cameraControls{cameraControls}
{
    using namespace nanogui;

    // re-define image view shader (using pre-multiplied alpha and blurring on low sample counts)
    {
        m_image_shader = new Shader(render_pass(), "imageview_shader", vertex_shader_imageview,
                                    fragment_shader_imageview, Shader::BlendMode::AlphaBlend);

        const float positions[] = {0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f};

        m_image_shader->set_buffer("position", VariableType::Float32, {6, 2}, positions);
        m_image_shader->set_uniform("blur", 0.0f);
        m_image_shader->set_uniform("useSRGB", false);
        m_image_shader->set_uniform("falsecolor", false);
    }

    sphereTracerControls = new Widget(toolbar);
    sphereTracerControls->set_layout(
        new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 4));

    (new Widget(sphereTracerControls))->set_fixed_width(8); // spacer
    auto icon = new Label(sphereTracerControls, utf8(FA_IMAGE));
    icon->set_font("icons");
    new Label(sphereTracerControls, "Sphere Tracer");
    (new Widget(sphereTracerControls))->set_fixed_width(16); // spacer

    auto renderMode =
        new ComboBox(sphereTracerControls, {"Depth", "Position", "Normal", "Num Steps"});
    renderMode->set_callback(
        [&](int i) -> void { params.mode = SphereTracerParameters::RenderMode{i}; });
    renderMode->set_selected_index(static_cast<int>(params.mode));
    renderMode->set_font_size(16);
    renderMode->set_side(Popup::Down);

    auto shape = new ComboBox(sphereTracerControls, {"Sphere", "Torus", "Octahedron"});
    shape->set_callback(
        [&](int i) -> void { params.sdf.function = SignedDistanceFunction::Type{i}; });
    shape->set_selected_index(static_cast<int>(params.sdf.function));
    shape->set_font_size(16);
    shape->set_side(Popup::Down);

    new Label(sphereTracerControls, "SPP");
    auto spp = new IntBox<uint16_t>(sphereTracerControls, params.maxSPP);
    spp->set_callback([&](uint16_t i) -> void { params.maxSPP = i; });
    spp->set_font_size(16);
    spp->set_fixed_width(60);
    spp->set_alignment(TextBox::Alignment::Right);
    spp->set_editable(true);
    spp->set_spinnable(true);
    spp->set_min_value(1);

    progress = new ProgressBar(sphereTracerControls);

    auto resetView = new Button(sphereTracerControls, "Reset View", FA_VECTOR_SQUARE);
    resetView->set_callback([&]() -> void { reset(); });
    resetView->set_font_size(16);

    set_pixel_callback([&](const Vector2i& index, char** out, size_t size) {
        const auto& pixels = sphereTracer.getFilm().getPixels();
        for (uint8_t ch = 0; ch < 4; ++ch) {
            const size_t i =
                static_cast<size_t>(index.x())
                + static_cast<size_t>(index.y()) * static_cast<size_t>(texture->size().x());
            const float value = pixels[i][ch];
            snprintf(out[ch], size, "%f", value);
        }
    });
}

void SphereTracerView::draw(NVGcontext* context)
{
    using nanogui::Texture;

    if (!texture || m_size != texture->size()) {
        texture =
            new Texture{Texture::PixelFormat::RGBA, Texture::ComponentFormat::Float32, m_size,
                        Texture::InterpolationMode::Trilinear, Texture::InterpolationMode::Nearest};
        set_image(texture);
    }
    else if (sphereTracer.imageHasChanged()) {
        const float sppRendered = sphereTracer.getSPPRendererd();
        const auto& pixels = sphereTracer.getFilm().getPixels();
        texture->upload(reinterpret_cast<const uint8_t*>(pixels.data()));

        progress->set_value(sppRendered / params.maxSPP);
        progress->set_tooltip(std::to_string(static_cast<uint32_t>(sppRendered)) + " / "s
                              + std::to_string(params.maxSPP));
        m_image_shader->set_uniform("blur",
                                    5.0f * std::exp(-sppRendered * (1.0f / std::log(5.0f))));
        m_image_shader->set_uniform("useSRGB", false);
        m_image_shader->set_uniform("falsecolor",
                                    params.mode == SphereTracerParameters::RenderMode::NumSteps);
    }
    ImageView::draw(context);

    const Resolution res{static_cast<uint32_t>(m_size.x()), static_cast<uint32_t>(m_size.y())};

    if (sphereTracer.setParams(params, cameraControls->getCameraParameters(res)))
        sphereTracer.start();
}
