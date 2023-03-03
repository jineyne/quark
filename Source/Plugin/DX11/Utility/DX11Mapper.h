#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/Buffer.h"

class DX11_EXPORT FDX11Mapper {
public:
    static uint32_t GetAccessFlags(EBufferUsage flag);
    static bool IsDynamic(EBufferUsage flag);
    static D3D11_USAGE GetUsage(EBufferUsage usage);
};
