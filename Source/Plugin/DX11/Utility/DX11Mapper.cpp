#include "DX11Mapper.h"

uint32_t FDX11Mapper::GetAccessFlags(EBufferUsage flag) {
    if (IsDynamic(flag)) {
        return D3D11_CPU_ACCESS_WRITE;
    }

    return 0;
}

bool FDX11Mapper::IsDynamic(EBufferUsage flag) {
    return flag == EBufferUsage::Dynamic;
}

D3D11_USAGE FDX11Mapper::GetUsage(EBufferUsage usage) {
    switch (usage) {
        case EBufferUsage::Dynamic:
            return D3D11_USAGE_DYNAMIC;

        default:
            return D3D11_USAGE_DEFAULT;
    }
}
