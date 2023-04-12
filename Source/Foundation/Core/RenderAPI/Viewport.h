#pragma once

#include "CorePrerequisites.h"
#include "Math/Rect.h"
#include "Image/Color.h"

enum class EClearFlags {
    Empty,
    Color = 1 << 0,
    Depth = 1 << 2,
    Stencil = 1 << 3,
};

ENUM_CLASS_FLAGS(EClearFlags)

class DLL_EXPORT FViewport {
protected:
    FRect mNormArea;

    EClearFlags mClearFlags;

    FColor mClearColorValue;

    float mClearDepthValue;

    uint16_t mClearStencilValue;

    FRenderTarget *mTarget = nullptr;

public:
    FViewport(float x = 0.0f, float y = 0.0f, float width = 1.0f, float height = 1.0f);
    virtual ~FViewport() = default;

public:
    static FViewport *New(float x = 0.0f, float y = 0.0f, float width = 1.0f, float height = 1.0f);

public:
    void setArea(const FRect &area);
    void setClearFlags(const EClearFlags &flags);
    void setClearValues(const FColor &clearColor, float clearDepth = 0.0f, uint32_t clearStencil = 0);
    void setClearDepthValue(float depth);
    void setClearStencilValue(uint16_t value);
    void setTarget(FRenderTarget *target);

    FRect getPixelArea() const;

    const auto &getArea() const { return mNormArea; }
    const auto &getClearFlags() const { return mClearFlags; }
    const auto &getClearColorValue() const { return mClearColorValue; }
    const auto &getClearDepthValue() const { return mClearDepthValue; }
    const auto &getClearStencilValue() const { return mClearStencilValue; }
    const auto &getTarget() const { return mTarget; }

protected:
    uint32_t getTargetWidth() const;
    uint32_t getTargetHeight() const;
};