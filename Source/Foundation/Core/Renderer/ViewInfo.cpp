#include "ViewInfo.h"

#include "RenderAPI/RenderTarget.h"
#include "SceneInfo.h"

ViewInfo::ViewInfo() : mTarget{}, mCamera(nullptr) {}

ViewInfo::ViewInfo(const ViewInfoDesc &desc)
        : mTarget(desc.target), mCamera(desc.camera) {
    mViewProjTransform = desc.viewTransform * desc.projTransform;
    setStateReductionMode(desc.stateReduction);
}

ViewInfo::~ViewInfo() {
    q_delete(mOpaqueQueue);
}

void ViewInfo::setStateReductionMode(EStateReduction reductionMode) {
    if (mOpaqueQueue != nullptr) {
        q_delete(mOpaqueQueue);
    }
    mOpaqueQueue = q_new<RenderQueue>(reductionMode);
}

void ViewInfo::setView(const ViewInfoDesc &desc) {
    mCamera = desc.camera;
    mViewProjTransform = desc.viewTransform * desc.projTransform;
    mTarget = desc.target;
}

void ViewInfo::beginFrame() {
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

void ViewInfo::endFrame() {
    mOpaqueQueue->clear();
}

void ViewInfo::determineVisible(const TArray<RenderableInfo *> &renderables, std::vector<bool> *visibility) {
    mVisibility.renderables.clear();
    mVisibility.renderables.resize(renderables.length(), false);

    const uint32_t count = renderables.length();

    // TODO: calc cull with frustom
    for (uint32_t i = 0; i < count; i++) {
        mVisibility.renderables[i] = true;
    }

    if (visibility != nullptr) {
        for (uint32_t i = 0; i < count; i++) {
            bool visible = (*visibility)[i];
            (*visibility)[i] = visible || mVisibility.renderables[i];
        }
    }
}

void ViewInfo::determineVisible(const TArray<RenderableLight *> &lights, std::vector<bool> *visibility) {
    mVisibility.lights.clear();
    mVisibility.lights.resize(lights.length(), false);

    const uint32_t count = lights.length();
    for (uint32_t i = 0; i < count; ++i) {
        mVisibility.lights[i] = true;
    }

    if (visibility != nullptr) {
        for (uint32_t i = 0; i < count; ++i) {
            (*visibility)[i] = (*visibility)[i] || mVisibility.lights[i];
        }
    }
}

void ViewInfo::queueRenderElements(const SceneData &sceneData) {
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

void ViewInfo::setTransform(const Vector3 &origin, const Vector3 &direction, const Matrix4 &view,
                            const Matrix4 &proj) {
    mViewProjTransform = view * proj;
}

ViewInfoGroup::ViewInfoGroup(ViewInfo **views, uint32_t viewCount, bool mainPass) : mMainPass(mainPass) {
    setViews(views, viewCount);
}

void ViewInfoGroup::setViews(ViewInfo **views, uint32_t viewCount) {
    mViewList.clear();

    for (uint32_t i = 0; i < viewCount; i++) {
        mViewList.add(views[i]);
    }
}

void ViewInfoGroup::determineVisibility(const SceneData &sceneData) {
    const uint32_t viewCount = mViewList.length();

    mVisibility.renderables.resize(sceneData.renderables.length(), false);
    mVisibility.renderables.assign(sceneData.renderables.length(), false);

    for (uint32_t i = 0; i < viewCount; i++) {
        mViewList[i]->determineVisible(sceneData.renderables, &mVisibility.renderables);
    }

    for (uint32_t i = 0; i < viewCount; i++) {
        mViewList[i]->queueRenderElements(sceneData);
    }

    mVisibility.lights.resize(sceneData.lights.length(), false);
    mVisibility.lights.assign(sceneData.lights.length(), false);

    for (uint32_t i = 0; i < viewCount; i++) {
        mViewList[i]->determineVisible(sceneData.lights, &mVisibility.lights);
    }
}
