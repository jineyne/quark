#include "Viewport.h"
#include "RenderTarget.h"

Viewport::Viewport(float x, float y, float width, float height)
        : mNormArea(x, y, width, height), mClearColorValue(Color::Green), mClearDepthValue(1.0f)
        , mClearStencilValue(0) {
    mClearFlags = EClearFlags::Color | EClearFlags::Depth;
}

Viewport *Viewport::New(float x, float y, float width, float height) {
    return q_new<Viewport>(x, y, width, height);
}

void Viewport::setArea(const Rect &area) {
    mNormArea = area;
}

void Viewport::setClearFlags(const EClearFlags &flags) {
    mClearFlags = flags;
}

void Viewport::setClearValues(const Color &clearColor, float clearDepth, uint32_t clearStencil) {
    mClearColorValue = clearColor;
    mClearDepthValue = clearDepth;
    mClearStencilValue = clearStencil;
}

void Viewport::setClearDepthValue(float depth) {
    mClearDepthValue = depth;
}

void Viewport::setClearStencilValue(uint16_t value) {
    mClearStencilValue = value;
}

void Viewport::setTarget(RenderTarget *target) {
    mTarget = target;
}

Rect Viewport::getPixelArea() const {
    float width = static_cast<float>(getTargetWidth());
    float height = static_cast<float>(getTargetHeight());

    Rect area{};
    area.x = mNormArea.x * width;
    area.y = mNormArea.y * height;
    area.width = mNormArea.width * width;
    area.height = mNormArea.height * height;

    return area;
}

uint32_t Viewport::getTargetWidth() const {
    if (mTarget == nullptr) {
        return 0;
    }

    return mTarget->getWidth();
}

uint32_t Viewport::getTargetHeight() const {
    if (mTarget == nullptr) {
        return 0;
    }

    return mTarget->getHeight();
}