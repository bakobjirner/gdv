#include <gui/gl_view.h>

#include <gui/camera_controls.h>
#include <render/camera.h>
#include <render/scene.h>
#include <string>

#include <nanogui/opengl.h>

using namespace std::string_literals;

GLView::GLView(nanogui::Widget* parent, Toolbar* toolbar, const Scene& scene,
               nanogui::ref<CameraControls> cameraControls)
    : Canvas{parent}, axesMesh{"../meshes/Axis.obj"},
      axesShader{axesGLMesh, {}, {Material::Diffuse{::Color{0.7f}}}}, cameraControls{cameraControls}
{
    using namespace nanogui;

    meshDisplayControls = new Widget(toolbar);
    meshDisplayControls->set_layout(
        new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 4));

    (new Widget(meshDisplayControls))->set_fixed_width(8); // spacer
    auto icon = new Label(meshDisplayControls, utf8(FA_DESKTOP));
    icon->set_font("icons");
    new Label(meshDisplayControls, "3D View");
    (new Widget(meshDisplayControls))->set_fixed_width(8); // spacer

    auto renderMode =
        new ComboBox(meshDisplayControls, {"My Little Shader", "Depth", "Position", "Normal", "UV",
                                           "Simple", "PBR", "Shadow Map", "Wobble"});
    renderMode->set_callback(
        [&](int i) -> void { params.mode = MeshDisplayParameters::RenderMode{i}; });
    renderMode->set_selected_index(static_cast<int>(params.mode));
    renderMode->set_font_size(16);
    renderMode->set_fixed_width(100);
    renderMode->set_side(Popup::Down);

    (new CheckBox(meshDisplayControls, "Wireframe", [&](bool b) -> void {
        params.wireframe = b;
    }))->set_checked(params.wireframe);
    (new CheckBox(meshDisplayControls, "rotate point light", [&](bool b) -> void {
        params.rotatePointLight = b;
    }))->set_checked(params.rotatePointLight);
    /*
    (new CheckBox(meshDisplayControls, "normal map", [&](bool b) -> void {
        params.pbrShaderParameters.normalMap = b;
    }))->set_checked(params.pbrShaderParameters.normalMap);
    (new CheckBox(meshDisplayControls, "displacement map", [&](bool b) -> void {
        params.pbrShaderParameters.displacementMap = b;
    }))->set_checked(params.pbrShaderParameters.displacementMap);
    (new CheckBox(meshDisplayControls, "shadow map", [&](bool b) -> void {
        params.pbrShaderParameters.shadowMap = b;
    }))->set_checked(params.pbrShaderParameters.shadowMap);
    */
    (new CheckBox(meshDisplayControls, "environment map", [&](bool b) -> void {
        params.pbrShaderParameters.envmap = b;
    }))->set_checked(params.pbrShaderParameters.envmap);
    /*
    new CheckBox(meshDisplayControls, "Show Axes", [&](bool b) -> void { params.showAxes = b; });
    new Label(meshDisplayControls, "BVH Level");
    auto bvhControl = new IntBox<int16_t>(meshDisplayControls, params.bvhLevel);
    bvhControl->set_callback([&](int16_t i) -> void { params.bvhLevel = i; });
    bvhControl->set_spinnable(true);
    bvhControl->set_min_max_values(0, BVH::maxDepth);
    bvhControl->set_font_size(16);
    bvhControl->set_fixed_width(60);

    new Label(meshDisplayControls, "roughness");
    auto roughness = new Slider(meshDisplayControls);
    roughness->set_callback([&](float x) -> void { params.pbrShaderParameters.alpha = x; });
    roughness->set_range({0.0f, 1.0f});
    roughness->set_value(0.0f);
    */

    new Label(meshDisplayControls, "blur");
    auto blur = new Slider(meshDisplayControls);
    blur->set_callback([&](float x) -> void { params.blur = static_cast<int16_t>(x); });
    blur->set_range({0.0f, 16.0f});
    blur->set_value(params.blur);

    setScene(scene);

    render_pass()->set_cull_mode(RenderPass::CullMode::Disabled);
}

void GLView::setScene(const Scene& scene)
{
    simpleShaders.clear();
    pbrShaders.clear();
    bvhs.clear();
    glMeshes.clear();

    for (const Instance& instance : scene.getInstances()) {
        bvhs.emplace_back(instance);
        glMeshes.emplace_back(std::make_unique<GLMesh>(instance.mesh));
        simpleShaders.emplace_back(*glMeshes.back(), instance.toWorld, instance.material);
        pbrShaders.emplace_back(*glMeshes.back(), instance.toWorld, instance.material);
    }
    for (const Light& light : scene.getLights()) {
        if (light.isPoint())
            params.pointLight = light.point();
    }

    sceneAABB = scene.getBounds();

    envmap = {scene.environmentMap};
}

void GLView::draw_contents()
{
    const float time = static_cast<float>(glfwGetTime());

    const Resolution res{static_cast<uint32_t>(m_size.x()), static_cast<uint32_t>(m_size.y())};
    const CameraParameters& cameraParams = cameraControls->getCameraParameters(res);

    Light::Point pointLight = params.pointLight;
    if (params.rotatePointLight) {
        // rotate the point light
        float sin = std::sin(time);
        float cos = std::cos(time);
        pointLight.pos.x = 0.8f * (params.pointLight.pos.x * cos + params.pointLight.pos.z * sin);
        pointLight.pos.z = 0.8f * (-params.pointLight.pos.x * sin + params.pointLight.pos.z * cos);
    }

    const CameraParameters lightCameraParams{
        pointLight.pos,
        sceneAABB.center()
            + cameraParams.up * dot(cameraParams.up, (sceneAABB.min - sceneAABB.center())) * 0.5f,
        cameraParams.computeFrame().dir,
        {90.0f},
        {},
        shadowResolution,
        cameraParams.tNear,
        distance(cameraParams.pos, cameraParams.target) * 5.0f,
        CameraParameters::CameraType::Perspective};

    const Matrix4D lp = lightCameraParams.projection() * lightCameraParams.viewTransformation();
    const Matrix4D view = cameraParams.viewTransformation();
    const Matrix4D proj = cameraParams.projection();
    const Matrix4D vp = proj * view;

    if (params.mode == MeshDisplayParameters::RenderMode::ShadowMap) {
        // render the shadow map's view
        for (auto& mesh : pbrShaders)
            mesh.drawShadow(lp);
        return;
    }

    if (params.mode == MeshDisplayParameters::RenderMode::PBR
        && params.pbrShaderParameters.shadowMap) {
        render_pass()->end();
        // render the shadow map
        shadowFramebuffer.bind();
        for (auto& mesh : pbrShaders)
            mesh.drawShadow(lp);
        shadowFramebuffer.unbind();
        render_pass()->begin();
    }

    const bool needsPostprocessing = (params.mode == MeshDisplayParameters::RenderMode::PBR
                                      || params.mode == MeshDisplayParameters::RenderMode::Simple
                                      || params.mode == MeshDisplayParameters::RenderMode::Wobble);

    if (needsPostprocessing) {
        render_pass()->end();
        if (primaryFramebuffer.getResolution() != res)
            primaryFramebuffer = {res, true, true, 4};
        CHECK_GL();
        primaryFramebuffer.bind();
        glDisable(GL_CULL_FACE);
    }

    if (params.mode == MeshDisplayParameters::RenderMode::PBR && params.pbrShaderParameters.envmap)
        envmapShader.draw(envmap, view);
    else
        backgroundShader.draw({static_cast<float>(m_size.x()), static_cast<float>(m_size.y())});

    if (params.mode == MeshDisplayParameters::RenderMode::ToyShader) {
        toyShader.draw(time);
        return;
    }

    if (params.showAxes) {
        axesShader.model = view;
        axesShader.draw(pointLight, cameraParams.pos, vp);
    }

    glPolygonMode(GL_FRONT_AND_BACK, params.wireframe ? GL_LINE : GL_FILL);

    if (params.mode == MeshDisplayParameters::RenderMode::PBR) {
        for (auto& mesh : pbrShaders)
            mesh.draw(pointLight, cameraParams.pos, vp, lp, shadowFramebuffer.getDepthTexture(),
                      envmap, params.pbrShaderParameters);
    }
    else if (params.mode != MeshDisplayParameters::RenderMode::ToyShader) {
        SimpleShader::DebugView debugView{SimpleShader::DebugView::None};
        bool wobble{false};

        switch (params.mode) {
        case MeshDisplayParameters::RenderMode::ToyShader:
        case MeshDisplayParameters::RenderMode::PBR:
        case MeshDisplayParameters::RenderMode::ShadowMap:
            // these cases are not handled here
        case MeshDisplayParameters::RenderMode::Simple:
            // no additional parameters required
            break;
        case MeshDisplayParameters::RenderMode::Depth:
            debugView = SimpleShader::DebugView::Depth;
            break;
        case MeshDisplayParameters::RenderMode::Position:
            debugView = SimpleShader::DebugView::Position;
            break;
        case MeshDisplayParameters::RenderMode::Normal:
            debugView = SimpleShader::DebugView::Normal;
            break;
        case MeshDisplayParameters::RenderMode::UV:
            debugView = SimpleShader::DebugView::UV;
            break;
        case MeshDisplayParameters::RenderMode::Wobble:
            wobble = true;
            break;
        }

        for (auto& mesh : simpleShaders)
            mesh.draw(pointLight, cameraParams.pos, vp, debugView, sceneAABB, wobble, time);
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (needsPostprocessing) {
        primaryFramebuffer.unbind();

        const Resolution halfRes{res.x / 2, res.y / 2};

        for (uint32_t i = 0; i < 3; ++i) {
            const Resolution& targetRes = i ? halfRes : res;
            if (auxillaryFramebuffers[i].getResolution() != targetRes)
                auxillaryFramebuffers[i] = {targetRes, true, !i, 1};
        }

        // convert multisampled framebuffer to single sample
        primaryFramebuffer.blitTo(auxillaryFramebuffers[0]);

        if (params.blur) {
            // copy over to half-res framebuffer
            auxillaryFramebuffers[0].blitTo(auxillaryFramebuffers[2]);

            // now, we can do some post-processing using our auxillary framebuffers
            // blur for a few iterations
            for (int16_t i = 0; i < params.blur; ++i) {
                auxillaryFramebuffers[1].bind();
                blurShader.draw(auxillaryFramebuffers[2].getColorTexture(), true);
                auxillaryFramebuffers[1].unbind();
                auxillaryFramebuffers[2].bind();
                blurShader.draw(auxillaryFramebuffers[1].getColorTexture(), false);
                auxillaryFramebuffers[2].unbind();
            }
        }

        // render out the final image
        render_pass()->begin();
        if (params.blur)
            compositionShader.draw(auxillaryFramebuffers[0].getColorTexture(),
                                   auxillaryFramebuffers[0].getDepthTexture(),
                                   auxillaryFramebuffers[2].getColorTexture(), cameraParams.tNear,
                                   cameraParams.tFar,
                                   distance(cameraParams.pos, cameraParams.target));
        else
            compositionShader.draw(auxillaryFramebuffers[0].getColorTexture(), {}, {}, 0.0f, 1.0f,
                                   1.0f);
    }

    if (params.bvhLevel)
        for (auto& bvh : bvhs)
            bvh.draw(static_cast<uint32_t>(params.bvhLevel), vp);
}
