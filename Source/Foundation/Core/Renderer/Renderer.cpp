#include "Renderer.h"
#include "RenderAPI/RenderAPI.h"
#include "ParamBlocks.h"

DEFINE_LOG_CATEGORY(LogRenderer)

void renderQueueElements(const TArray<RenderQueueElement> &elements) {
    auto &rapi = RenderAPI::Instance();

    for (const auto &entry : elements) {
        if (entry.applyPass) {
            const auto *material = entry.element->material;
            const auto *pass = material->getPass(entry.passIdx, entry.techniqueIdx);

            rapi.setGraphicsPipeline(pass->getGraphicsPipelineState());
        }

        auto *gpuParams = entry.element->params->getGpuParams(entry.passIdx);
        if (gpuParams != nullptr) {
            rapi.setGpuParams(gpuParams);
        }

        entry.element->draw();
    }
}

Renderer::Renderer() : mCallbacks(&CompareCallback) {
    mSceneInfo = q_new<SceneInfo>();
    mMainViewGroup = q_new<ViewInfoGroup>(nullptr, 0, true);
}

bool Renderer::CompareCallback(const RendererExtension *a, const RendererExtension *b) {
    // Sort by alpha setting first, then by cull mode, then by index
    if (a->getLocation() == b->getLocation()) {
        if (a->getPriority() == b->getPriority()) {
            return a > b; // Use address, at this point it doesn't matter, but std::set requires us to differentiate
        } else {
            return a->getPriority() > b->getPriority();
        }
    } else {
        return static_cast<uint32_t>(a->getLocation()) < static_cast<uint32_t>(b->getLocation());
    }
}

void Renderer::update() {

}

void Renderer::renderAll() {
    const SceneData &data = mSceneInfo->getSceneData();

    auto &rapi = RenderAPI::Instance();

    for (const auto & info : data.renderTargetInfos) {
        TArray<ViewInfo *> views;
        const auto &cameras = info.cameras;

        const auto cameraCount = static_cast<uint32_t>(cameras.length());
        for (uint32_t i = 0; i < cameraCount; i++) {
            const auto viewIdx = data.cameraToView[cameras[i]];
            auto viewInfo = data.views[viewIdx];
            views.add(viewInfo);
        }

        mMainViewGroup->setViews(*views, static_cast<uint32_t>(views.length()));
        mMainViewGroup->determineVisibility(data);

        const bool anythinDrawn = renderViews(views);
        if (info.target->isWindow() && anythinDrawn) {
            rapi.swapBuffer(info.target);
        }
    }
}

void Renderer::notifyRenderableCreated(Renderable *renderable) {
    mSceneInfo->registerRenderable(renderable);
}

void Renderer::notifyRenderableUpdated(Renderable *renderable) {
    mSceneInfo->updateRenderable(renderable, 0);
}

void Renderer::notifyRenderableRemoved(Renderable *renderable) {
    mSceneInfo->unregisterRenderable(renderable);
}

void Renderer::notifyCameraCreated(CameraBase *camera) {
    mSceneInfo->registerCamera(camera);
}

void Renderer::notifyCameraUpdated(CameraBase *camera) {
    mSceneInfo->updateCamera(camera, 0);
}

void Renderer::notifyCameraRemoved(CameraBase *camera) {
    mSceneInfo->unregisterCamera(camera);
}

void Renderer::addPlugin(RendererExtension *extension) {
    mCallbacks.insert(extension);
}

void Renderer::removePlugin(RendererExtension *extension) {
    mCallbacks.erase(extension);
}

void Renderer::updateCameraRenderTargets(CameraBase *camera, bool removed) {
    auto viewport = camera->getViewport();
    auto target = viewport->getTarget();
}

void Renderer::notifyLightCreated(LightBase *light) {
    mSceneInfo->registerLight(light);
}

void Renderer::notifyLightUpdated(LightBase *light) {
    mSceneInfo->updateLight(light, 0);
}

void Renderer::notifyLightRemoved(LightBase *light) {
    mSceneInfo->unregisterLight(light);
}

void Renderer::renderExtensions(CameraBase *camera, ERenderLocation location) {
    auto &rapi = gRenderAPI();

    mActiveViewport = camera->getViewport();
    rapi.setRenderTarget(mActiveViewport->getTarget());
    rapi.setViewport(mActiveViewport->getArea());

    if (!mCallbacks.empty()) {
        for (auto &extension : mCallbacks) {
            if (!extension->checkCamera(camera)) {
                continue;
            }

            if (extension->getLocation() != location) {
                continue;
            }

            extension->render(camera);
        }
    }
}

bool Renderer::renderViews(TArray<ViewInfo *> &views) {
    bool anythingDraw = false;

    const auto viewCount = static_cast<uint32_t>(views.length());

    for (uint32_t i = 0; i < viewCount; i++) {
        auto *view = views[i];

        renderView(view);
        anythingDraw = true;
    }

    /*if (!mCallbacks.empty()) {
        for (auto &extension : mCallbacks) {
            if (!extension->checkCamera(camera)) {
                continue;
            }

            if (extension->getLocation() != ERenderLocation::Overlay) {
                continue;
            }

            extension->render(camera);
        }
    }*/

    return anythingDraw;
}

void Renderer::renderView(ViewInfo *view) {
    const auto &data = mSceneInfo->getSceneData();

    view->beginFrame();

    const auto targetData = view->getTargetData();

    auto &rapi = RenderAPI::Instance();
    rapi.setRenderTarget(targetData.target);
    rapi.setViewport(targetData.viewRect);

    rapi.clearRenderTarget((EFrameBufferType) targetData.clearFlags, targetData.clearColor);

    CameraBase *sceneCamera = view->getSceneCamera();
    if (sceneCamera != nullptr) {
        //
    }

    renderExtensions(sceneCamera, ERenderLocation::Prepare);
    renderExtensions(sceneCamera, ERenderLocation::PreBasePass);

    const VisibilityInfo &visibility = view->getVisibilityMasks();

    const auto lightCount = std::min<uint32_t>(data.lights.length(), STANDARD_FORWARD_MAX_NUM_LIGHTS);
    TArray<LightData> lightDataList(STANDARD_FORWARD_MAX_NUM_LIGHTS);

    for (uint32_t i = 0; i < lightCount; i++) {
        if (!visibility.lights[i]) {
            continue;
        }

        auto *light = data.lights[i];

        light->getParameters(lightDataList[i]);
    }

    data.gLightsBuffer->writeData(0, sizeof(LightData) * lightDataList.length(), lightDataList.getData(),
                                  EBufferWriteType::Discard);

    for (uint32_t i = 0; i < data.renderables.length(); i++) {
        if (!visibility.renderables[i]) {
            continue;
        }

        // update lights
        for (uint32_t j = 0; j < data.renderables[i]->elements.length(); j++) {
            auto &element = data.renderables[i]->elements[j];
            auto params = element.params->getGpuParams(0);

            if (params == nullptr) {
                continue;
            }

            params->setParam(EGpuProgramType::Fragment, TEXT("gLightCount"), (int) lightCount);

            GpuParamBinding binding;
            params->getParamInfo()->getBinding(EGpuProgramType::Fragment,
                                               FGpuPipelineParamInfoBase::ParamType::Buffer,
                                               TEXT("gLights"), binding);

            if (binding.slot != (uint32_t) -1) {
                params->setBuffer(binding.set, binding.slot, data.gLightsBuffer);
            }
        }

        auto *renderable = data.renderables[i];
        renderable->updatePerCallBuffer(view->getViewProjTransform());
    }

    const auto &opaqueElements = view->getOpaqueQueue()->getSortedElementList();
    renderQueueElements(opaqueElements);

    renderExtensions(sceneCamera, ERenderLocation::PostBasePass);

    renderExtensions(sceneCamera, ERenderLocation::Overlay);

    rapi.setRenderTarget(nullptr);
    view->endFrame();
}

Renderer &gRenderer() {
    return Renderer::Instance();
}
