#include "DX11GpuBuffer.h"
#include "DX11Buffer.h"

static void DeleteBuffer(FBuffer *buffer) {
    delete static_cast<FDX11Buffer *>(buffer);
}

FDX11GpuBuffer::FDX11GpuBuffer(const FGpuBufferDesc &desc) : FGpuBuffer(desc) {
    mBuffer = q_new<FDX11Buffer>(D3D11_BIND_SHADER_RESOURCE, mSize, mUsage);
    mBufferDeleter = &DeleteBuffer;
}

ID3D11Buffer *FDX11GpuBuffer::getBuffer() const {
    return static_cast<FDX11Buffer *>(mBuffer)->getBuffer();
}
