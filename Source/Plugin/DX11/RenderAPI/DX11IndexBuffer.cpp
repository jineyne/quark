#include "DX11IndexBuffer.h"
#include "DX11Buffer.h"

static void DeleteBuffer(FBuffer *buffer) {
    delete static_cast<FDX11Buffer *>(buffer);
}

FDX11IndexBuffer::FDX11IndexBuffer(const FIndexBufferDesc &desc) : FIndexBuffer(desc) {
    mBuffer = q_new<FDX11Buffer>(D3D11_BIND_INDEX_BUFFER, mSize, mUsage);
    mBufferDeleter = &DeleteBuffer;
}

ID3D11Buffer *FDX11IndexBuffer::getBuffer() const {
    return static_cast<FDX11Buffer *>(mBuffer)->getBuffer();
}
