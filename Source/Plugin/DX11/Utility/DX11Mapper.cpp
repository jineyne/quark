#include "DX11Mapper.h"
#include "Exception/Exception.h"

D3D11_COMPARISON_FUNC FDX11Mapper::Get(ECompareFunction cf) {
    switch(cf) {
        case ECompareFunction::AlwaysFail:
            return D3D11_COMPARISON_NEVER;
        case ECompareFunction::AlwaysPass:
            return D3D11_COMPARISON_ALWAYS;
        case ECompareFunction::Less:
            return D3D11_COMPARISON_LESS;
        case ECompareFunction::LessEqual:
            return D3D11_COMPARISON_LESS_EQUAL;
        case ECompareFunction::Equal:
            return D3D11_COMPARISON_EQUAL;
        case ECompareFunction::NotEqual:
            return D3D11_COMPARISON_NOT_EQUAL;
        case ECompareFunction::GreaterEqual:
            return D3D11_COMPARISON_GREATER_EQUAL;
        case ECompareFunction::Greater:
            return D3D11_COMPARISON_GREATER;
    };

    // Unsupported type
    return D3D11_COMPARISON_ALWAYS;
}

D3D11_STENCIL_OP FDX11Mapper::Get(EStencilOperation op, bool invert) {
    switch(op) {
        case EStencilOperation::Keep:
            return D3D11_STENCIL_OP_KEEP;
        case EStencilOperation::Zero:
            return D3D11_STENCIL_OP_ZERO;
        case EStencilOperation::Replace:
            return D3D11_STENCIL_OP_REPLACE;
        case EStencilOperation::Increment:
            return invert ? D3D11_STENCIL_OP_DECR_SAT : D3D11_STENCIL_OP_INCR_SAT;
        case EStencilOperation::Decrement:
            return invert ? D3D11_STENCIL_OP_INCR_SAT : D3D11_STENCIL_OP_DECR_SAT;
        case EStencilOperation::IncrementWrap:
            return invert ? D3D11_STENCIL_OP_DECR : D3D11_STENCIL_OP_INCR;
        case EStencilOperation::DecrementWrap:
            return invert ? D3D11_STENCIL_OP_INCR : D3D11_STENCIL_OP_DECR;
        case EStencilOperation::Invert:
            return D3D11_STENCIL_OP_INVERT;
    }

    // Unsupported type
    return D3D11_STENCIL_OP_KEEP;
}

D3D11_MAP FDX11Mapper::Get(EBufferWriteType type) {
    switch (type) {
        case EBufferWriteType::Normal:
            return D3D11_MAP_WRITE;

        default:
        case EBufferWriteType::Discard:
            return D3D11_MAP_WRITE_DISCARD;

        case EBufferWriteType::NoOverWrite:
            return D3D11_MAP_WRITE_NO_OVERWRITE;
    }
}

LPCSTR FDX11Mapper::Get(EVertexElementSemantic format) {
    switch (format) {
        case EVertexElementSemantic::Position:
            return "POSITION";

        case EVertexElementSemantic::Normal:
            return "NORMAL";

        case EVertexElementSemantic::Color:
            return "COLOR";

        case EVertexElementSemantic::TexCoord:
            return "TEXCOORD";
    }

    return "";
}

EVertexElementSemantic FDX11Mapper::Get(LPCSTR semantic) {
    if (strcmp(semantic, "COLOR") == 0) {
        return EVertexElementSemantic::Color;
    } else if (strcmp(semantic, "NORMAL") == 0) {
        return EVertexElementSemantic::Normal;
    } else if (strcmp(semantic, "POSITION") == 0) {
        return EVertexElementSemantic::Position;
    } else if (strcmp(semantic, "TEXCOORD") == 0) {
        return EVertexElementSemantic::TexCoord;
    }

    // Unsupported type
    return EVertexElementSemantic::Position;
}

DXGI_FORMAT FDX11Mapper::Get(EPixelFormat format) {
    switch (format) {
        case EPixelFormat::R8:
            return DXGI_FORMAT_R8_UNORM;

        case EPixelFormat::RG8:
            return DXGI_FORMAT_R8G8_UNORM;

        case EPixelFormat::RGB8:
        case EPixelFormat::RGBA8:
            return DXGI_FORMAT_R8G8B8A8_UNORM;

        case EPixelFormat::BGR8:
            return DXGI_FORMAT_B8G8R8X8_UNORM;

        case EPixelFormat::BGRA8:
            return DXGI_FORMAT_B8G8R8A8_UNORM;

        case EPixelFormat::R16F:
            return DXGI_FORMAT_R16_FLOAT;

        case EPixelFormat::RG16F:
            return DXGI_FORMAT_R16G16_FLOAT;

        case EPixelFormat::RGB16F:
        case EPixelFormat::RGBA16F:
            return DXGI_FORMAT_R16G16B16A16_FLOAT;

        case EPixelFormat::R32F:
            return DXGI_FORMAT_R32_FLOAT;

        case EPixelFormat::RG32F:
            return DXGI_FORMAT_R32G32_FLOAT;

        case EPixelFormat::RGB32F:
        case EPixelFormat::RGBA32F:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
    }
}

DXGI_FORMAT FDX11Mapper::Get(EVertexElementType type) {
    switch (type) {
        case EVertexElementType::Color:
            return DXGI_FORMAT_R8G8B8A8_UNORM;
        case EVertexElementType::Float1:
            return DXGI_FORMAT_R32_FLOAT;
        case EVertexElementType::Float2:
            return DXGI_FORMAT_R32G32_FLOAT;
        case EVertexElementType::Float3:
            return DXGI_FORMAT_R32G32B32_FLOAT;
        case EVertexElementType::Float4:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case EVertexElementType::Int1:
            return DXGI_FORMAT_R32_SINT;
    }

    // Unsupported type
    return DXGI_FORMAT_R32G32B32A32_FLOAT;
}

DXGI_FORMAT FDX11Mapper::Get(EGpuBufferFormat format) {
    static bool lookupInitialized = false;

    static DXGI_FORMAT lookup[(int) EGpuBufferFormat::Count];
    if (!lookupInitialized)
    {
        lookup[(int) EGpuBufferFormat::_16x1f] = DXGI_FORMAT_R16_FLOAT;
        lookup[(int) EGpuBufferFormat::_16x2f] = DXGI_FORMAT_R16G16_FLOAT;
        lookup[(int) EGpuBufferFormat::_16x4f] = DXGI_FORMAT_R16G16B16A16_FLOAT;
        lookup[(int) EGpuBufferFormat::_32x1f] = DXGI_FORMAT_R32_FLOAT;
        lookup[(int) EGpuBufferFormat::_32x2f] = DXGI_FORMAT_R32G32_FLOAT;
        lookup[(int) EGpuBufferFormat::_32x3f] = DXGI_FORMAT_R32G32B32_FLOAT;
        lookup[(int) EGpuBufferFormat::_32x4f] = DXGI_FORMAT_R32G32B32A32_FLOAT;
        lookup[(int) EGpuBufferFormat::_8x1] = DXGI_FORMAT_R8_UNORM;
        lookup[(int) EGpuBufferFormat::_8x2] = DXGI_FORMAT_R8G8_UNORM;
        lookup[(int) EGpuBufferFormat::_8x4] = DXGI_FORMAT_R8G8B8A8_UNORM;
        lookup[(int) EGpuBufferFormat::_16x1] = DXGI_FORMAT_R16_UNORM;
        lookup[(int) EGpuBufferFormat::_16x2] = DXGI_FORMAT_R16G16_UNORM;
        lookup[(int) EGpuBufferFormat::_16x4] = DXGI_FORMAT_R16G16B16A16_UNORM;
        lookup[(int) EGpuBufferFormat::_8x1s] = DXGI_FORMAT_R8_SINT;
        lookup[(int) EGpuBufferFormat::_8x2s] = DXGI_FORMAT_R8G8_SINT;
        lookup[(int) EGpuBufferFormat::_8x4s] = DXGI_FORMAT_R8G8B8A8_SINT;
        lookup[(int) EGpuBufferFormat::_16x1s] = DXGI_FORMAT_R16_SINT;
        lookup[(int) EGpuBufferFormat::_16x2s] = DXGI_FORMAT_R16G16_SINT;
        lookup[(int) EGpuBufferFormat::_16x4s] = DXGI_FORMAT_R16G16B16A16_SINT;
        lookup[(int) EGpuBufferFormat::_32x1s] = DXGI_FORMAT_R32_SINT;
        lookup[(int) EGpuBufferFormat::_32x2s] = DXGI_FORMAT_R32G32_SINT;
        lookup[(int) EGpuBufferFormat::_32x3s] = DXGI_FORMAT_R32G32B32_SINT;
        lookup[(int) EGpuBufferFormat::_32x4s] = DXGI_FORMAT_R32G32B32A32_SINT;
        lookup[(int) EGpuBufferFormat::_8x1u] = DXGI_FORMAT_R8_UINT;
        lookup[(int) EGpuBufferFormat::_8x2u] = DXGI_FORMAT_R8G8_UINT;
        lookup[(int) EGpuBufferFormat::_8x4u] = DXGI_FORMAT_R8G8B8A8_UINT;
        lookup[(int) EGpuBufferFormat::_16x1u] = DXGI_FORMAT_R16_UINT;
        lookup[(int) EGpuBufferFormat::_16x2u] = DXGI_FORMAT_R16G16_UINT;
        lookup[(int) EGpuBufferFormat::_16x4u] = DXGI_FORMAT_R16G16B16A16_UINT;
        lookup[(int) EGpuBufferFormat::_32x1u] = DXGI_FORMAT_R32_UINT;
        lookup[(int) EGpuBufferFormat::_32x2u] = DXGI_FORMAT_R32G32_UINT;
        lookup[(int) EGpuBufferFormat::_32x3u] = DXGI_FORMAT_R32G32B32_UINT;
        lookup[(int) EGpuBufferFormat::_32x4u] = DXGI_FORMAT_R32G32B32A32_UINT;

        lookupInitialized = true;
    }

    if ((int) format >= (int) EGpuBufferFormat::Count)
        return DXGI_FORMAT_UNKNOWN;

    return lookup[(UINT32)format];
}

D3D11_MAP FDX11Mapper::GetLockOptions(EGpuLockOptions lockOptions) {
    switch(lockOptions) {
        case EGpuLockOptions::WriteOnlyNoOverWrite:
            return D3D11_MAP_WRITE_NO_OVERWRITE;
            break;
        case EGpuLockOptions::ReadWrite:
            return D3D11_MAP_READ_WRITE;
            break;
        case EGpuLockOptions::WriteOnlyDiscard:
            return D3D11_MAP_WRITE_DISCARD;
            break;
        case EGpuLockOptions::ReadOnly:
            return D3D11_MAP_READ;
            break;
        case EGpuLockOptions::WriteOnly:
            return D3D11_MAP_WRITE;
            break;
        default:
            break;
    };

    EXCEPT(FLogDX11, RenderAPIException, TEXT("Invalid lock option. No DX11 equivalent of: %ld"), static_cast<int>(lockOptions));
    return D3D11_MAP_WRITE;
}

EVertexElementType FDX11Mapper::GetInputType(D3D_REGISTER_COMPONENT_TYPE type) {
    switch (type) {
        case D3D_REGISTER_COMPONENT_FLOAT32:
            return EVertexElementType::Float4;
            case D3D_REGISTER_COMPONENT_SINT32:
                return EVertexElementType::Int4;
            case D3D_REGISTER_COMPONENT_UINT32:
                return EVertexElementType::Uint4;
        default:
            return EVertexElementType::Float4;
    }
}

uint32_t FDX11Mapper::GetAccessFlags(EBufferUsage flag) {
    if (IsDynamic(flag)) {
        return D3D11_CPU_ACCESS_WRITE;
    }

    return 0;
}

bool FDX11Mapper::IsDynamic(EBufferUsage flag) {
    return flag == EBufferUsage::Dynamic;
}

uint32_t FDX11Mapper::GetSizeInBytes(EPixelFormat pf, uint32_t width, uint32_t height) {
    if(FPixelUtil::IsCompressed(pf)) {
        UINT32 blockWidth = FMath::DivideAndRoundUp(width, 4U);
        UINT32 blockHeight = FMath::DivideAndRoundUp(height, 4U);

        return blockWidth * blockHeight * 16;
    } else {
        return width * height * FPixelUtil::GetNumElemBytes(pf);
    }
}

D3D11_USAGE FDX11Mapper::GetUsage(EBufferUsage usage) {
    switch (usage) {
        case EBufferUsage::Dynamic:
            return D3D11_USAGE_DYNAMIC;

        default:
            return D3D11_USAGE_DEFAULT;
    }
}
