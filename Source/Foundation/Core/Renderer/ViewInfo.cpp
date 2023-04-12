#include "ViewInfo.h"

#include "RenderAPI/RenderTarget.h"
#include "SceneInfo.h"

FViewInfo::FViewInfo() : mTarget{}, mCamera(nullptr) {}

FViewInfo::FViewInfo(const FViewInfoDesc &desc)
        : mTarget(desc.target), mCamera(desc.camera) {
    mViewProjTransform = desc.projTransform * desc.viewTransform;
    setStateReductionMode(desc.stateReduction);
}

FViewInfo::~FViewInfo() {
    q_delete(mOpaqueQueue);
}

void FViewInfo::setStateReductionMode(EStateReduction reductionMode) {
    if (mOpaqueQueue != nullptr) {
        q_delete(mOpaqueQueue);
    }
    mOpaqueQueue = q_new<FRenderQueue>(reductionMode);
}

void FViewInfo::setView(const FViewInfoDesc &desc) {
    mCamera = desc.camera;
    mViewProjTransform = desc.projTransform * desc.viewTransform;
    mTarget = desc.target;
}

void FViewInfo::beginFrame() {
    bool perViewBufferDirty = false;

    if (mCamera) {
        const auto &viewport = mCamera->getViewport();
        if (viewport) {
            uint32_t newTargetWidth = 0;
            uint32_t newTargetHeight = 0;
            if (mTarget.target != nullptr) {
                newTargetWidth = mTarget.target->getWidth();
                newTargetHeight = mTarget.target->getHeight();
            }

            if (newTargetWidth != mTarget.targetWidth || newTargetHeight != mTarget.targetHeight) {
                mTarget.viewRect = viewport->getPixelArea();
                mTarget.targetWidth = newTargetWidth;
                mTarget.targetHeight = newTargetHeight;

                perViewBufferDirty = true;
            }
        }
    }
}

void FViewInfo::endFrame() {
    mOpaqueQueue->clear();
}

void FViewInfo::determineVisible(const TArray<FRenderableInfo *> &renderables, std::vector<bool> *visibility) {
    mVisibility.renderables.clear();
    mVisibility.renderables.resize(renderables.length(), false);

    const uint32_t renderableCount = renderables.length();

    // TODO: calc cull with frustom
    for (uint32_t i = 0; i < renderableCount; i++) {
        mVisibility.renderables[i] = true;
    }

    if (visibility != nullptr) {
        for (uint32_t i = 0; i < renderableCount; i++) {
            bool visible = (*visibility)[i];
            (*visibility)[i] = visible || mVisibility.renderables[i];
        }
    }
}

void FViewInfo::queueRenderElements(const FSceneData &sceneData) {
    const uint32_t renderableCount = static_cast<uint32_t>(sceneData.renderables.length());
    for (uint32_t i = 0; i < renderableCount; i++) {
        if (!mVisibility.renderables[i]) {
            continue;
        }

        for (auto &element : sceneData.renderables[i]->elements) {
            uint32_t techniqueIdx = element.defaultTechniqueIdx;
            mOpaqueQueue->add(&element, 0, techniqueIdx);
        }
    }

    mOpaqueQueue->sort();
}

FViewInfoGroup::FViewInfoGroup(FViewInfo **views, uint32_t viewCount, bool mainPass) : mMainPass(mainPass) {
    setViews(views, viewCount);
}

void FViewInfoGroup::setViews(FViewInfo **views, uint32_t viewCount) {
    mViewList.clear();

    for (uint32_t i = 0; i < viewCount; i++) {
        mViewList.add(views[i]);
    }
}

void FViewInfoGroup::determineVisibility(const FSceneData &sceneData) {
    const uint32_t viewCount = mViewList.length();

    mVisibility.renderables.resize(sceneData.renderables.length(), false);
    mVisibility.renderables.assign(sceneData.renderables.length(), false);

    for (uint32_t i = 0; i < viewCount; i++) {
        mViewList[i]->determineVisible(sceneData.renderables, &mVisibility.renderables);
    }

    for (uint32_t i = 0; i < viewCount; i++) {
        mViewList[i]->queueRenderElements(sceneData);
    }
}
