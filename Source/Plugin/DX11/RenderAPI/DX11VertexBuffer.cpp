#include "DX11VertexBuffer.h"
#include "DX11Buffer.h"

static void DeleteBuffer(FBuffer *buffer) {
    delete static_cast<FDX11Buffer *>(buffer);
}

FDX11VertexBuffer::FDX11VertexBuffer(const FVertexBufferDesc &desc) : FVertexBuffer(desc) {
    mBuffer = q_new<FDX11Buffer>(EBufferType::Vertex, mUsage, desc.vertexCount, desc.vertexSize);
    mBufferDeleter = &DeleteBuffer;
}

ID3D11Buffer *FDX11VertexBuffer::getBuffer() const {
    return static_cast<FDX11Buffer *>(mBuffer)->getBuffer();
}
