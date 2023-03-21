#include "TextureSurface.h"

const FTextureSurface FTextureSurface::Complete = FTextureSurface(0, 0, 0, 0);

FTextureSurface::FTextureSurface(uint32_t mipLevel, uint32_t mipLevelsCount, uint32_t face, uint32_t faceCount)
        : mipLevel(mipLevel), mipLevelsCount(mipLevelsCount), face(face), faceCount(faceCount) {}