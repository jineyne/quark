#include "DX11VertexBuffer.h"
#include "DX11Buffer.h"

static void DeleteBuffer(FBuffer *buffer) {
    delete static_cast<FDX11Buffer *>(buffer);
}

FDX11VertexBuffer::FDX11VertexBuffer(const FVertexBufferDesc &desc) : FVertexBuffer(desc) {
    mBuffer = new FDX11Buffer(D3D11_BIND_VERTEX_BUFFER, mSize, mUsage);
    mBufferDeleter = &DeleteBuffer;
}

ID3D11Buffer *FDX11VertexBuffer::getBuffer() const {
    return static_cast<FDX11Buffer *>(mBuffer)->getBuffer();
}
