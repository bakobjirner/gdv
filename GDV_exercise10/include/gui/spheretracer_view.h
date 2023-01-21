#ifndef SPHERETRACER_VIEW_H
#define SPHERETRACER_VIEW_H

#include <nanogui/nanogui.h>

#include <gui/camera_controls.h>
#include <gui/gui.h>
#include <render/spheretracer.h>

class SphereTracerView final : public nanogui::ImageView {
public:
    SphereTracerView(nanogui::Widget* parent, Toolbar* toolbar, SphereTracer& sphereTracer,
                     nanogui::ref<CameraControls> cameraControls);

    virtual void draw(NVGcontext* context) override;

private:
    SphereTracer& sphereTracer;
    nanogui::ref<nanogui::Texture> texture;
    nanogui::ref<CameraControls> cameraControls;
    nanogui::ref<nanogui::Widget> sphereTracerControls;

    nanogui::ref<nanogui::ProgressBar> progress;

    SphereTracerParameters params;
};

#endif // SPHERETRACER_VIEW_H
