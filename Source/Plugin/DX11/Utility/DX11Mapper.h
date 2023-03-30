#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/Buffer.h"
#include "RenderAPI/VertexDeclaration.h"
#include "Image/PixelUtil.h"

class DX11_EXPORT FDX11Mapper {
public:
    static D3D11_COMPARISON_FUNC Get(ECompareFunction cf);
    static D3D11_STENCIL_OP Get(EStencilOperation op, bool invert = false);

    static D3D11_MAP Get(EBufferWriteType type);
    static LPCSTR Get(EVertexElementSemantic format);
    static EVertexElementSemantic Get(LPCSTR semantic);
    static DXGI_FORMAT Get(EPixelFormat format);
    static DXGI_FORMAT Get(EVertexElementType type);
    static D3D11_MAP GetLockOptions(EGpuLockOptions lockOptions);
    static EVertexElementType GetInputType(D3D_REGISTER_COMPONENT_TYPE type);
    static uint32_t GetAccessFlags(EBufferUsage flag);
    static bool IsDynamic(EBufferUsage flag);
    static uint32_t GetSizeInBytes(EPixelFormat pf, uint32_t width = 1, uint32_t height = 1);
    static D3D11_USAGE GetUsage(EBufferUsage usage);
};
