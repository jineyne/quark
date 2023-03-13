#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/Buffer.h"
#include "RenderAPI/VertexDeclaration.h"

class DX11_EXPORT FDX11Mapper {
public:
    static D3D11_MAP Get(EBufferWriteType type);
    static LPCSTR Get(EVertexElementSemantic format);
    static EVertexElementSemantic Get(LPCSTR semantic);
    static DXGI_FORMAT Get(EVertexElementType type);
    static D3D11_MAP GetLockOptions(EGpuLockOptions lockOptions);
    static EVertexElementType GetInputType(D3D_REGISTER_COMPONENT_TYPE type);
    static uint32_t GetAccessFlags(EBufferUsage flag);
    static bool IsDynamic(EBufferUsage flag);
    static D3D11_USAGE GetUsage(EBufferUsage usage);
};
