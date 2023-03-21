#pragma once

#include "CorePrerequisites.h"

class DLL_EXPORT FTextureSurface {
public:
    static const FTextureSurface Complete;

    uint32_t mipLevel;
    uint32_t mipLevelsCount;
    uint32_t face;
    uint32_t faceCount;

public:
    FTextureSurface(uint32_t mipLevel = 0, uint32_t mipLevelsCount = 1, uint32_t face = 0, uint32_t faceCount = 1);
};
