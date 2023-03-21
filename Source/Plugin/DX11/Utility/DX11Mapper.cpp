#include "DX11Mapper.h"
#include "Exception/Exception.h"

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
