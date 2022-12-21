#ifndef GL_VIEW_H
#define GL_VIEW_H

#include <geometry/mesh.h>
#include <gui/gui.h>
#include <render/gl/gl_envmap.h>
#include <render/gl/helper_shader.h>
#include <render/gl/pbr_shader.h>
#include <render/gl/simple_shader.h>

struct MeshDisplayParameters {
    enum class RenderMode {
        ToyShader,
        Depth,
        Position,
        Normal,
        UV,
        Simple,
        PBR,
        ShadowMap,
        Wobble
    };

    bool wireframe{false};
    bool showAxes{false};
    bool rotatePointLight{false};
    int16_t bvhLevel{0};

    RenderMode mode{RenderMode::PBR};

    PBRShader::Parameters pbrShaderParameters;

    Light::Point pointLight{};
};

/// A class to display 3D meshes
class GLView final : public nanogui::Canvas {
public:
    GLView(nanogui::Widget* parent, Toolbar* toolbar, const Scene& scene,
           nanogui::ref<CameraControls> cameraControls);

    void setScene(const Scene& scene);

    virtual void draw_contents() override;

private:
    BackgroundShader backgroundShader;
    MyLittleShader toyShader;

    Mesh axesMesh;
    GLMesh axesGLMesh{axesMesh};
    SimpleShader axesShader;
    AABB sceneAABB;

    nanogui::ref<CameraControls> cameraControls;
    nanogui::ref<nanogui::Widget> meshDisplayControls;

    std::vector<std::unique_ptr<GLMesh>> glMeshes;
    std::vector<SimpleShader> simpleShaders;
    std::vector<PBRShader> pbrShaders;

    std::vector<BVHShader> bvhs;
    MeshDisplayParameters params;

    Resolution shadowResolution{2048, 2048};
    GLFramebuffer shadowFramebuffer{shadowResolution, false, true};

    GLEnvironmentMap envmap;
    EnvmapShader envmapShader;
};

#endif // GL_VIEW_H
