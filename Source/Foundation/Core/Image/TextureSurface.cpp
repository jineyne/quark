#include "TextureSurface.h"

const TextureSurface TextureSurface::Complete = TextureSurface(0, 0, 0, 0);

TextureSurface::TextureSurface(uint32_t mipLevel, uint32_t mipLevelsCount, uint32_t face, uint32_t faceCount)
        : mipLevel(mipLevel), mipLevelsCount(mipLevelsCount), face(face), faceCount(faceCount) {}