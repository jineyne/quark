#include "Renderer.h"
#include "RenderAPI/RenderAPI.h"
#include "ParamBlocks.h"

DEFINE_LOG_CATEGORY(FLogRenderer)

void renderQueueElements(const TArray<FRenderQueueElement> &elements) {
    auto &rapi = FRenderAPI::Instance();

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

FRenderer::FRenderer() : mCallbacks(&CompareCallback) {
    mSceneInfo = q_new<FSceneInfo>();
    mMainViewGroup = q_new<FViewInfoGroup>(nullptr, 0, true);
}

bool FRenderer::CompareCallback(const FRendererExtension *a, const FRendererExtension *b) {
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

void FRenderer::update() {

}

void FRenderer::renderAll() {
    const FSceneData &data = mSceneInfo->getSceneData();

    auto &rapi = FRenderAPI::Instance();

    for (const auto & info : data.renderTargetInfos) {
        TArray<FViewInfo *> views;
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

void FRenderer::notifyRenderableCreated(FRenderable *renderable) {
    mSceneInfo->registerRenderable(renderable);
}

void FRenderer::notifyRenderableUpdated(FRenderable *renderable) {
    mSceneInfo->updateRenderable(renderable, 0);
}

void FRenderer::notifyRenderableRemoved(FRenderable *renderable) {
    mSceneInfo->unregisterRenderable(renderable);
}

void FRenderer::notifyCameraCreated(FCameraBase *camera) {
    mSceneInfo->registerCamera(camera);
}

void FRenderer::notifyCameraRemoved(FCameraBase *camera) {
    mSceneInfo->unregisterCamera(camera);
}

void FRenderer::addPlugin(FRendererExtension *extension) {
    mCallbacks.insert(extension);
}

void FRenderer::removePlugin(FRendererExtension *extension) {
    mCallbacks.erase(extension);
}

void FRenderer::updateCameraRenderTargets(FCameraBase *camera, bool removed) {
    auto viewport = camera->getViewport();
    auto target = viewport->getTarget();
}

void FRenderer::renderOverlay(FCameraBase *camera) {
    auto &rapi = gRenderAPI();

    mActiveViewport = camera->getViewport();
    rapi.setRenderTarget(mActiveViewport->getTarget());
    rapi.setViewport(mActiveViewport->getArea());

    if (!mCallbacks.empty()) {
        for (auto &extension : mCallbacks) {
            if (!extension->checkCamera(camera)) {
                continue;
            }

            if (extension->getLocation() != ERenderLocation::Overlay) {
                continue;
            }

            extension->render(camera);
        }
    }
}

bool FRenderer::renderViews(TArray<FViewInfo *> &views) {
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

void FRenderer::renderView(FViewInfo *view) {
    const auto &data = mSceneInfo->getSceneData();

    view->beginFrame();

    const auto targetData = view->getTargetData();

    auto &rapi = FRenderAPI::Instance();
    rapi.setRenderTarget(targetData.target);
    rapi.setViewport(targetData.viewRect);

    rapi.clearRenderTarget((EFrameBufferType) targetData.clearFlags, targetData.clearColor);

    FCameraBase *sceneCamera = view->getSceneCamera();
    if (sceneCamera != nullptr) {
        //
    }

    const FVisibilityInfo &visibility = view->getVisibilityMasks();
    const auto renderableCount = data.renderables.length();
    for (uint32_t i = 0; i < renderableCount; i++) {
        if (!visibility.renderables[i]) {
            continue;
        }

        auto *renderable = data.renderables[i];
        renderable->updatePerCallBuffer(view->getViewProjTransform());
    }

    const auto &opaqueElements = view->getOpaqueQueue()->getSortedElementList();
    renderQueueElements(opaqueElements);

    rapi.setRenderTarget(nullptr);
    view->endFrame();
}

FRenderer &gRenderer() {
    return FRenderer::Instance();
}
