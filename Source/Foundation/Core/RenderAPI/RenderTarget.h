#pragma once

#include "CorePrerequisites.h"

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
