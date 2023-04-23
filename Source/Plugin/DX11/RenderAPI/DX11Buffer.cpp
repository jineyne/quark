#include "DX11Buffer.h"

#include "RenderAPI/DX11Device.h"
#include "RenderAPI/DX11RenderAPI.h"
#include "Utility/DX11Mapper.h"

FDX11Buffer::FDX11Buffer(EBufferType bufferType, EBufferUsage usage, uint32_t elementCount, uint32_t elementSize)
        : FBuffer(elementSize * elementCount, usage), mBufferType(bufferType) {
    bool isLoadStore = (usage & EBufferUsage::LoadStore) == EBufferUsage::LoadStore;

    mDesc.Usage = FDX11Mapper::GetUsage(usage);
    mDesc.ByteWidth = getSize();
    mDesc.BindFlags = 0;
    mDesc.CPUAccessFlags = FDX11Mapper::GetAccessFlags(usage);
    mDesc.MiscFlags = 0;
    mDesc.StructureByteStride = 0;

    switch (bufferType) {
        case EBufferType::Standard:
            mDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            break;

        case EBufferType::Structured:
            mDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            mDesc.StructureByteStride = elementSize;
            mDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
            break;

        case EBufferType::Raw:
            mDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            mDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
            break;

        case EBufferType::Vertex:
            mDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            break;

        case EBufferType::Index:
            mDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            break;

        case EBufferType::Constant:
            mDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            break;

        case EBufferType::InDirectArgument:
            mDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            mDesc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
            break;
    }

    if(isLoadStore) {
        mDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    }

    auto rapi = FRenderAPI::InstancePtr();
    auto d3drapi = static_cast<FDX11RenderAPI *>(rapi);

    auto device = d3drapi->getPrimaryDevice();
    auto dxdevice = device->getDevice();
    HR(dxdevice->CreateBuffer(&mDesc, nullptr, &mBuffer));
}

FDX11Buffer::~FDX11Buffer() {
    SAFE_RELEASE(mBuffer);
}

void FDX11Buffer::writeData(uint32_t offset, uint32_t length, const void *src, EBufferWriteType flags) {
    checkf(offset + length <= mSize, TEXT("Out of Bound!: %ld (offset: %ld + length: %ld) < %ld"), offset + length, offset, length, mSize);

    auto rapi = FRenderAPI::InstancePtr();
    auto d3drapi = static_cast<FDX11RenderAPI *>(rapi);

    auto device = d3drapi->getPrimaryDevice();
    auto dxdevice = device->getDevice();

    if(mDesc.Usage == D3D11_USAGE_DYNAMIC || mDesc.Usage == D3D11_USAGE_STAGING) {
        EGpuLockOptions lockOption = EGpuLockOptions::WriteOnly;
        if(flags == EBufferWriteType::Discard) {
            lockOption = EGpuLockOptions::WriteOnlyDiscard;
        } else if(flags == EBufferWriteType::NoOverWrite) {
            lockOption = EGpuLockOptions::WriteOnlyNoOverWrite;
        }

        void* dst = this->lock(offset, length, lockOption);
        memcpy(dst, src, length);
        this->unlock();
    } else if(mDesc.Usage == D3D11_USAGE_DEFAULT) {
        if (mBufferType == EBufferType::Constant) {
            assert(offset == 0);
            device->getImmediateContext()->UpdateSubresource(mBuffer, 0, nullptr, src, 0, 0);
        }
        else
        {
            D3D11_BOX dstBox;
            dstBox.left = (UINT)offset;
            dstBox.right = (UINT)offset + length;
            dstBox.top = 0;
            dstBox.bottom = 1;
            dstBox.front = 0;
            dstBox.back = 1;

            device->getImmediateContext()->UpdateSubresource(mBuffer, 0, &dstBox, src, 0, 0);
        }
    } else {
        LOG(FLogDX11, Error, TEXT("Trying to write into a buffer with unsupported usage: %ld"), mDesc.Usage);
    }
}

void *FDX11Buffer::map(uint32_t offset, uint32_t length, const EGpuLockOptions &options) {
    if (offset + length > mSize) {
        LOG(FLogDX11, Error, TEXT("Offset + length is out of bounds"));
        return nullptr;
    }

    auto rapi = FRenderAPI::InstancePtr();
    auto d3drapi = static_cast<FDX11RenderAPI *>(rapi);

    D3D11_MAP mapType;
    if ((mDesc.Usage == D3D11_USAGE_DYNAMIC && (options & EGpuLockOptions::ReadOnly) != EGpuLockOptions::ReadOnly) || mDesc.Usage == D3D11_USAGE_STAGING) {
        switch (options) {
            case EGpuLockOptions::WriteOnlyDiscard:
                if (mUsage == EBufferUsage::Dynamic) {
                    mapType = D3D11_MAP_WRITE_DISCARD;
                } else {
                    // Map cannot be called with MAP_WRITE_DISCARD access, because the Resource was not created as
                    // D3D11_USAGE_DYNAMIC. D3D11_USAGE_DYNAMIC Resources must use either MAP_WRITE_DISCARD
                    // or MAP_WRITE_NO_OVERWRITE with Map.
                    mapType = D3D11_MAP_WRITE;
                }
                break;
            case EGpuLockOptions::WriteOnlyNoOverWrite:
                if (mBufferType == EBufferType::Index || mBufferType == EBufferType::Vertex) {
                    mapType = D3D11_MAP_WRITE_NO_OVERWRITE;
                } else {
                    mapType = D3D11_MAP_WRITE;
                }
                break;
            case EGpuLockOptions::WriteOnly:
                mapType = D3D11_MAP_WRITE;
                break;
            case EGpuLockOptions::ReadWrite:
                if ((mDesc.CPUAccessFlags & D3D11_CPU_ACCESS_READ) != 0 && (mDesc.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE) != 0) {
                    mapType = D3D11_MAP_READ_WRITE;
                } else if (mDesc.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE) {
                    mapType = D3D11_MAP_WRITE;
                } else {
                    mapType = D3D11_MAP_READ;
                }
                break;
            default:
            case EGpuLockOptions::ReadOnly:
                mapType = D3D11_MAP_READ;
                break;
        }

    }

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ID3D11DeviceContext *context = d3drapi->getPrimaryDevice()->getImmediateContext();
    HR(context->Map(mBuffer, 0, mapType, 0, &mappedResource));

    return (void *) (((char *) mappedResource.pData) + offset);
}

void FDX11Buffer::unmap() {
    auto rapi = FRenderAPI::InstancePtr();
    auto d3drapi = static_cast<FDX11RenderAPI *>(rapi);

    ID3D11DeviceContext *context = d3drapi->getPrimaryDevice()->getImmediateContext();
    context->Unmap(mBuffer, 0);
}
