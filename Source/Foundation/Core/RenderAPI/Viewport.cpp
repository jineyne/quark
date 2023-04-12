#include "Viewport.h"
#include "RenderTarget.h"

FViewport::FViewport(float x, float y, float width, float height)
        : mNormArea(x, y, width, height), mClearColorValue(FColor::Green), mClearDepthValue(1.0f)
        , mClearStencilValue(0) {
    mClearFlags = EClearFlags::Color | EClearFlags::Depth;
}

FViewport *FViewport::New(float x, float y, float width, float height) {
    return q_new<FViewport>(x, y, width, height);
}

void FViewport::setArea(const FRect &area) {
    mNormArea = area;
}

void FViewport::setClearFlags(const EClearFlags &flags) {
    mClearFlags = flags;
}

void FViewport::setClearValues(const FColor &clearColor, float clearDepth, uint32_t clearStencil) {
    mClearColorValue = clearColor;
    mClearDepthValue = clearDepth;
    mClearStencilValue = clearStencil;
}

void FViewport::setClearDepthValue(float depth) {
    mClearDepthValue = depth;
}

void FViewport::setClearStencilValue(uint16_t value) {
    mClearStencilValue = value;
}

void FViewport::setTarget(FRenderTarget *target) {
    mTarget = target;
}

FRect FViewport::getPixelArea() const {
    float width = static_cast<float>(getTargetWidth());
    float height = static_cast<float>(getTargetHeight());

    FRect area{};
    area.x = mNormArea.x * width;
    area.y = mNormArea.y * height;
    area.width = mNormArea.width * width;
    area.height = mNormArea.height * height;

    return area;
}

uint32_t FViewport::getTargetWidth() const {
    if (mTarget == nullptr) {
        return 0;
    }

    return mTarget->getWidth();
}

uint32_t FViewport::getTargetHeight() const {
    if (mTarget == nullptr) {
        return 0;
    }

    return mTarget->getHeight();
}