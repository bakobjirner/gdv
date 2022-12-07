/*
    Framework for GDV exercises, based on NanoGUI.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <iostream>
#include <nanogui/nanogui.h>
// includes windows.h on Windows
#include <nanogui/opengl.h>

#include <render/material.h>
#include <render/raytracer.h>
#include <render/scene.h>

#include <gui/camera_controls.h>
#include <gui/gl_view.h>
#include <gui/gui.h>
#include <gui/raytracer_view.h>

class GDVApplication final : public nanogui::Screen {
public:
    GDVApplication(const nanogui::Vector2i& size)
        : nanogui::Screen{size,
                          "GDV 2022/23",
                          /* resizable */ true,
                          /* fullscreen */ false,
                          /* depth_buffer */ true,
                          /* stencil_buffer */ true,
                          /* float_buffer */ false,
                          /* gl_major */ 4,
                          /* gl_minor */ 1}
    {
        inc_ref();

        gui = new GDVGUI(this);

        // setup the scene
        {
            // selection of Materials
            struct MaterialDatabase {
                const Material::Diffuse boxRed{{0.8f, 0.4f, 0.3f}};
                const Material::Diffuse boxGreen{{0.4f, 0.8f, 0.3f}};
                const Material::Diffuse boxWhite{{0.5f, 0.5f, 0.5f}};

                const Material::Diffuse diffuseGray{{0.5f, 0.5f, 0.5f}};
                const Material::Diffuse diffuseBlue{{0.2f, 0.5f, 0.9f}};
                const Material::Dielectric water{1.33f, 1.0f};
                const Material::Dielectric glass{1.5f, 1.0f};
                const Material::Conductor gold{{0.143085f, 0.374852f, 1.44208f},
                                               {3.98205f, 2.38506f, 1.60276f}};
                const Material::Conductor silver{{0.15522f, 0.116692f, 0.138342f},
                                                 {4.827f, 3.12139f, 2.14636f}};
                const Material::Conductor copper{{0.20038f, 0.923777f, 1.10191f},
                                                 {3.91185f, 2.45217f, 2.14159f}};
                const Material::GGX glossyMicrofacet{0.1f};
                const Material::GGX roughMicrofacet{0.3f};
                const Material::GGX veryRoughMicrofacet{0.3f};
                const Material::RoughConductor roughGold{gold, roughMicrofacet};
                const Material::RoughPlastic blueRubber{diffuseBlue, glass, roughMicrofacet};
                const Material::RoughPlastic redRubber{boxRed, glass, glossyMicrofacet};
                const Material::RoughPlastic stone{diffuseGray, glass, veryRoughMicrofacet};

                const Material texturedStone{stone,
                                             {},
                                             {{"../textures/stone_wall_diff_2k.jpg"},
                                              {"../textures/stone_wall_nor_gl_2k.jpg"},
                                              {"../textures/stone_wall_rough_2k.jpg"},
                                              {"../textures/stone_wall_disp_2k.jpg"}}};

                const Material blueEmitter{diffuseBlue, diffuseBlue.albedo * 10.0f};
            } materials;

            Scene scene;

            // Instances for scene
            scene.addInstance({"../meshes/CubeTop.obj", {materials.boxWhite}});
            scene.addInstance({"../meshes/CubeBack.obj", {materials.texturedStone}});
            scene.addInstance({"../meshes/CubeBottom.obj", {materials.boxWhite}});
            scene.addInstance({"../meshes/CubeLeft.obj", {materials.boxRed}});
            scene.addInstance({"../meshes/CubeRight.obj", {materials.boxGreen}});

            // selection of models...
            // TODO change materials if you wish
            scene.addInstance({"../meshes/big_bunny.obj",
                               {materials.roughGold},
                               {Matrix3D::scale(0.5f), {0.0f, -0.017f, -0.5f}}});
            scene.addInstance(
                {"../meshes/gdv.obj", {materials.diffuseBlue}, {{}, {0.0f, 0.5f, -1.0f}}});
            scene.addInstance({"../meshes/uv_sphere.obj",
                               {materials.texturedStone},
                               {Matrix3D::scale(0.8f), {0.9f, 0.4f, -1.0f}}});
            // scene.addInstance({"../meshes/loki.obj", gold});
            // scene.addInstance({"../meshes/Su_Laegildah.obj", diffuseGray});

            scene.addPointLight({300.0f, {0.0f, 2.5f, 2.0f}});
            // TODO: enable the 3D view, if you want
            gui->addMeshView(scene);
            // rayTracer.setScene(scene);
            // gui->addRayTracerView(rayTracer);
        }

        perform_layout();
        resize_event(framebuffer_size());
    }

    bool resize_event(const nanogui::Vector2i& size) override
    {
        gui->set_fixed_size(framebuffer_size());
        perform_layout();
        Screen::resize_event(size);

        return true;
    }

private:
    nanogui::ref<GDVGUI> gui;
    RayTracer rayTracer;
};

int main(int /* argc */, char** /* argv */)
{
    try {
        nanogui::init();

        /* scoped variables */ {
            nanogui::ref<GDVApplication> app = new GDVApplication({1024, 768});
            app->dec_ref();
            app->draw_all();
            app->set_visible(true);
            nanogui::mainloop(1 / 60.f * 1000);
        }

        nanogui::shutdown();
    }
    catch (const std::exception& e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
#if defined(_WIN32)
        MessageBoxA(nullptr, error_msg.c_str(), NULL, MB_ICONERROR | MB_OK);
#else
        std::cerr << error_msg << std::endl;
#endif
        return -1;
    }
    catch (...) {
        std::cerr << "Caught an unknown error!" << std::endl;
    }

    return 0;
}
