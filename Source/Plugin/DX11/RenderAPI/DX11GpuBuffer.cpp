#include "DX11GpuBuffer.h"

#include "DX11Buffer.h"
#include "DX11Device.h"
#include "DX11RenderAPI.h"
#include "Utility/DX11Mapper.h"

static void DeleteBuffer(Buffer *buffer) {
    delete static_cast<DX11Buffer *>(buffer);
}

DX11GpuBuffer::DX11GpuBuffer(const GpuBufferDesc &desc) : GpuBuffer(desc) {
    mBuffer = q_new<DX11Buffer>(desc.type, mUsage, desc.elementCount, desc.elementSize);
    mBufferDeleter = &DeleteBuffer;

    DX11RenderAPI* rapi = static_cast<DX11RenderAPI*>(RenderAPI::InstancePtr());

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));

    if (desc.type == EBufferType::Standard) {
        srvDesc.Format = DX11Mapper::Get(desc.format);
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvDesc.Buffer.FirstElement = 0;
        srvDesc.Buffer.NumElements = desc.elementCount;
    } else if (desc.type == EBufferType::Structured) {
        srvDesc.Format = DXGI_FORMAT_UNKNOWN;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvDesc.Buffer.FirstElement = 0;
        srvDesc.Buffer.NumElements = desc.elementCount;
    } else if (desc.type == EBufferType::InDirectArgument) {
        srvDesc.Format = DXGI_FORMAT_R32_UINT;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvDesc.Buffer.ElementOffset = 0;
        srvDesc.Buffer.ElementWidth = desc.elementSize;
    }

    HRESULT hr = rapi->getPrimaryDevice()->getDevice()->CreateShaderResourceView(getBuffer(), &srvDesc, &mShadeResourceView);
    if (FAILED(hr) || rapi->getPrimaryDevice()->hasError()) {
        String msg = rapi->getPrimaryDevice()->getErrorDescription();
        EXCEPT(LogDX11, RenderAPIException, TEXT("Cannot create ShaderResourceView: %ls"), *msg);
    }

    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
    ZeroMemory(&uavDesc, sizeof(uavDesc));

    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

    if (desc.type == EBufferType::Standard) {
        uavDesc.Format = DX11Mapper::Get(desc.format);
        uavDesc.Buffer.FirstElement = 0;
        uavDesc.Buffer.NumElements = desc.elementCount;
        uavDesc.Buffer.Flags = 0;
    } else if (desc.type == EBufferType::Structured) {
        uavDesc.Format = DXGI_FORMAT_UNKNOWN;
        uavDesc.Buffer.FirstElement = 0;
        uavDesc.Buffer.NumElements = desc.elementCount;
        uavDesc.Buffer.Flags = 0;
    } else if (desc.type == EBufferType::InDirectArgument) {
        uavDesc.Format = DXGI_FORMAT_R32_UINT;
        uavDesc.Buffer.Flags = 0;
        uavDesc.Buffer.FirstElement = 0;
        uavDesc.Buffer.NumElements = desc.elementSize;
    }

    hr = rapi->getPrimaryDevice()->getDevice()->CreateUnorderedAccessView(getBuffer(), &uavDesc, &mUnorderedAccessView);

    if (FAILED(hr) || rapi->getPrimaryDevice()->hasError()) {
        String msg = rapi->getPrimaryDevice()->getErrorDescription();
        EXCEPT(LogDX11, RenderAPIException, TEXT("Cannot create UnorderedAccessView: %ls"), *msg);
    }
}

ID3D11Buffer *DX11GpuBuffer::getBuffer() const {
    return static_cast<DX11Buffer *>(mBuffer)->getBuffer();
}

ID3D11ShaderResourceView *DX11GpuBuffer::getShadeResourceView() const {
    return mShadeResourceView;
}

ID3D11UnorderedAccessView *DX11GpuBuffer::getUnorderedAccessView() const {
    return mUnorderedAccessView;
}
