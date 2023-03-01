#pragma once

#include "CorePrerequisites.h"

enum class EFrameBufferType : uint8_t {
    Color = 1 << 0,
    Depth = 1 << 1,
    Stencil = 1 << 2
};

ENUM_CLASS_FLAGS(EFrameBufferType)

class DLL_EXPORT FRenderTarget {
protected:
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;

public:
    virtual void swapBuffers(uint32_t mask = 0xffffffff) = 0;

    virtual bool isWindow() { return false; }

    const auto &getWidth() const { return mWidth; }
    const auto &getHeight() const { return mHeight; }
};
