#include "DX11GpuParamBlockBuffer.h"

#include "DX11Buffer.h"
#include "DX11RenderAPI.h"

DX11GpuParamBlockBuffer::DX11GpuParamBlockBuffer(uint32_t size, EBufferUsage usage)
        : GpuParamBlockBuffer(size, usage) {
    DX11RenderAPI* d3d11rs = static_cast<DX11RenderAPI*>(RenderAPI::InstancePtr());
    DX11Device *device = d3d11rs->getPrimaryDevice();

    mBuffer = q_new<DX11Buffer>(EBufferType::Constant, usage, 1, size);
}

DX11GpuParamBlockBuffer::~DX11GpuParamBlockBuffer() {
    if (mBuffer) {
        delete static_cast<DX11Buffer *>(mBuffer);
    }
}

ID3D11Buffer *DX11GpuParamBlockBuffer::getBuffer() const {
    return static_cast<DX11Buffer *>(mBuffer)->getBuffer();
}
