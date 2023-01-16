/*
    Framework for GDV exercises, based on NanoGUI.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

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

#include <iostream>
#include <random>
#include <string>

using namespace std::string_literals;

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

        { // 3D rendering

            // selection of Materials
            struct MaterialDatabase {
                const Material::Diffuse boxRed{{0.8f, 0.4f, 0.3f}};
                const Material::Diffuse boxGreen{{0.4f, 0.8f, 0.3f}};
                const Material::Diffuse boxWhite{{0.5f, 0.5f, 0.5f}};

                const Material::Diffuse diffuseGray{{0.5f, 0.5f, 0.5f}};
                const Material::Diffuse diffuseBlue{{0.2f, 0.5f, 0.9f}};
                const Material::Diffuse diffuseWhite{{1.0f, 1.0f, 1.0f}};
                const Material::Diffuse diffuseGreen{{0.4f, 0.9f, 0.3f}};
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
                const Material::RoughConductor roughGold{gold, roughMicrofacet};
                const Material::RoughPlastic greenRubber{diffuseGreen, glass, roughMicrofacet};
                const Material::RoughPlastic blueRubber{diffuseBlue, glass, roughMicrofacet};
                const Material::RoughPlastic redRubber{boxRed, glass, glossyMicrofacet};
                const Material::RoughPlastic stone{diffuseGray, glass, roughMicrofacet};
                const Material::RoughPlastic metal{diffuseGray, water, glossyMicrofacet};
                const Material::RoughPlastic snowParam{diffuseWhite, glass, roughMicrofacet};

                const Material stoneWall{stone,
                                         {{"../textures/stone_wall_diff_2k.jpg"},
                                          {"../textures/stone_wall_nor_gl_2k.jpg"},
                                          {"../textures/stone_wall_rough_2k.jpg"},
                                          {},
                                          {"../textures/stone_wall_disp_2k.jpg"}}};
                const Material cannon{metal,
                                      {{"../textures/cannon_01_diff_2k.jpg"},
                                       {"../textures/cannon_01_nor_gl_2k.jpg"},
                                       {"../textures/cannon_01_metal_2k.jpg"},
                                       {"../textures/cannon_01_rough_2k.jpg"}}};
                const Material treasure{metal,
                                        {{"../textures/treasure_chest_diff_2k.jpg"},
                                         {"../textures/treasure_chest_nor_gl_2k.jpg"},
                                         {"../textures/treasure_chest_metal_2k.jpg"},
                                         {"../textures/treasure_chest_rough_2k.jpg"}}};
                const Material snow{snowParam,
                                        {{},
                                         {},
                                         {},
                                         {},
                                         {},
                                        0,
                                        -0.5,
                                        8
                                         }};
                

                const Material blueEmitter{diffuseBlue, {}, diffuseBlue.albedo * 10.0f};
            } const materials;

            // setup the scene
            Scene scene;

            if constexpr (/* Christmas Special */ (true)) {
                const std::array<std::string, 6> christmasMeshes{
                    "../meshes/christmas_special/customized_christmas_tree_100_75_5_9.obj"s,
                    "../meshes/christmas_special/customized_christmas_tree_100_75_7_3.obj"s,
                    "../meshes/christmas_special/customized_christmas_tree_100_75_20_4.obj"s,
                    "../meshes/christmas_special/customized_christmas_tree_100_75_20_100.obj"s,
                    "../meshes/christmas_special/customized_christmas_tree_150_80_6_5.obj"s,
                    "../meshes/christmas_special/customized_christmas_tree_150_100_7_100.obj"s};

                const std::array<Material, 10> christmasMaterials{{{materials.greenRubber},
                                                                   {materials.greenRubber},
                                                                   {materials.greenRubber},
                                                                   {materials.greenRubber},
                                                                   {materials.blueRubber},
                                                                   {materials.boxRed},
                                                                   {materials.boxGreen},
                                                                   {materials.roughGold},
                                                                   {materials.gold},
                                                                   {materials.silver}}};

                const uint32_t numInstances = 256;

                std::default_random_engine prng;
                prng.seed(031); // oct 31 == dec 25 :)
                std::uniform_int_distribution<uint32_t> randomMesh(0, christmasMeshes.size() - 1);
                std::uniform_int_distribution<uint32_t> randomMaterial(0, christmasMaterials.size()
                                                                              - 1);
                std::uniform_real_distribution<float> randomScale(0.005f, 0.015f);
                std::uniform_real_distribution<float> randomX(-3.0f, 8.0f);
                std::uniform_real_distribution<float> randomZ(-2.0f, 8.0f);

                for (uint32_t i = 0; i < numInstances; ++i) {
                    const float scale{randomScale(prng)};
                    Point3D position{randomX(prng), 0.0f, randomZ(prng)};
                    position *= 1.2f;

                    scene.addInstance({christmasMeshes.at(randomMesh(prng)),
                                       christmasMaterials.at(randomMaterial(prng)),
                                       {Matrix3D::scale(scale), position}});
                }

                gui->setCameraParameters({{-0.5f, 1.0f, -0.5f}, {2.0f, 1.5f, 3.0f}});

                //settings for snow
                float snowSpawnWidth = 5;
                float snowSpawnDepth = 5;
                float snowStartHeight = 3.0f;
                float maxPosVariance = 5.0f;
                float snowSpeed = -0.5f;
                float speedVariance = 0.3f;
                float maxPos = 10;
                for(int i = 0; i < 50; i++){
                    //generate a random number between -0,5*width and +0,5*width
                    float myXPos = (float)(rand()) / (float)(RAND_MAX) * snowSpawnWidth - snowSpawnWidth/2;
                    //generate a random number between -0,5*depth and +0,5*depth
                    float myZPos = (float)(rand()) / (float)(RAND_MAX) * snowSpawnDepth - snowSpawnDepth/2;
                    //generate a random number between  maxPos and maxPosVariance + maxPos
                    float myMaxPos = (float)(rand()) / (float)(RAND_MAX) * maxPosVariance + maxPos;
                    //generate a random number between  speed and speedVariance + speed
                    float mySpeed = (float)(rand()) / (float)(RAND_MAX) * speedVariance + snowSpeed;

                    //individual material settings for variance
                    Material snow1{materials.snowParam,
                                        {{},
                                         {},
                                         {},
                                         {},
                                         {},
                                        0,
                                        mySpeed,
                                        myMaxPos
                                         }};

                    //generate snow particle
                    scene.addInstance({"../meshes/snowflake.obj",snow1,{Matrix3D::scale(1.0f), {myXPos, snowStartHeight, myZPos}}});
                }

                scene.environmentMap = {"../textures/blaubeuren_church_square_2k.hdr", false};
            }
            else {
                // Instances for scene
                if constexpr (/* Cornell Box */ (true)) {
                    scene.addInstance({"../meshes/CubeTop.obj", {materials.boxWhite}});
                    scene.addInstance({"../meshes/CubeBack.obj", materials.stoneWall});
                    scene.addInstance({"../meshes/CubeBottom.obj", {materials.boxWhite}});
                    scene.addInstance({"../meshes/CubeLeft.obj", {materials.boxRed}});
                    scene.addInstance({"../meshes/CubeRight.obj", {materials.boxGreen}});
                }

                // selection of models...
                // TODO change materials if you wish
                // scene.addInstance({"../meshes/big_bunny.obj", {materials.roughGold},
                //                   {Matrix3D::scale(0.5f), {0.0f, -0.017f, -0.5f}}});
                // scene.addInstance({"../meshes/gdv.obj", {materials.diffuseBlue},
                //                   {{}, {0.0f, 0.5f, -1.0f}}});
                // scene.addInstance({"../meshes/uv_sphere.obj", materials.stoneWall,
                //                   {Matrix3D::scale(0.8f), {0.9f, 0.4f, -1.0f}}});
                // scene.addInstance({"../meshes/uv_sphere.obj", {materials.silver},
                //                   {{}, {0.0f, 1.0f, 0.0f}}});
                // scene.addInstance({"../meshes/cannon_01_2k.obj", materials.cannon});
                // scene.addInstance({"../meshes/treasure_chest_2k.obj",
                //                    materials.treasure,
                //                    {{}, {1.0f, 0.0f, -1.0f}}});
                // scene.addInstance({"../meshes/teapot.obj",
                //                    {materials.silver},
                //                    {Matrix3D::scale(0.1f), {-1.3f, 0.0f, 0.0f}}});
                // scene.addInstance({"../meshes/loki.obj", {materials.gold}});
                // scene.addInstance({"../meshes/Su_Laegildah.obj", {materials.diffuseGray}});

                scene.environmentMap = {"../textures/kloofendal_48d_partly_cloudy_2k.hdr", false};
            }

            scene.addPointLight({300.0f, {0.0f, 2.5f, 2.0f}});

            // TODO: enable the 3D view, if you want
            gui->addGLView(scene);
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
