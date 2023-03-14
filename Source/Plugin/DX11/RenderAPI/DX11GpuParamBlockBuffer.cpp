#include "DX11GpuParamBlockBuffer.h"

#include "DX11Buffer.h"
#include "DX11RenderAPI.h"

FDX11GpuParamBlockBuffer::FDX11GpuParamBlockBuffer(uint32_t size, EBufferUsage usage)
        : FGpuParamBlockBuffer(size, usage) {
    FDX11RenderAPI* d3d11rs = static_cast<FDX11RenderAPI*>(FRenderAPI::InstancePtr());
    FDX11Device *device = d3d11rs->getPrimaryDevice();

    mBuffer = new FDX11Buffer(D3D11_BIND_CONSTANT_BUFFER, size, usage);
}

FDX11GpuParamBlockBuffer::~FDX11GpuParamBlockBuffer() {
    if (mBuffer) {
        delete static_cast<FDX11Buffer *>(mBuffer);
    }
}

ID3D11Buffer *FDX11GpuParamBlockBuffer::getBuffer() const {
    return static_cast<FDX11Buffer *>(mBuffer)->getBuffer();
}