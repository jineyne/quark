#include "DX11VertexBuffer.h"
#include "DX11Buffer.h"

static void DeleteBuffer(Buffer *buffer) {
    delete static_cast<DX11Buffer *>(buffer);
}

DX11VertexBuffer::DX11VertexBuffer(const VertexBufferDesc &desc) : VertexBuffer(desc) {
    mBuffer = q_new<DX11Buffer>(EBufferType::Vertex, mUsage, desc.vertexCount, desc.vertexSize);
    mBufferDeleter = &DeleteBuffer;
}

ID3D11Buffer *DX11VertexBuffer::getBuffer() const {
    return static_cast<DX11Buffer *>(mBuffer)->getBuffer();
}
