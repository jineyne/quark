#include "DX11IndexBuffer.h"
#include "DX11Buffer.h"

static void DeleteBuffer(Buffer *buffer) {
    delete static_cast<DX11Buffer *>(buffer);
}

DX11IndexBuffer::DX11IndexBuffer(const FIndexBufferDesc &desc) : IndexBuffer(desc) {
    mBuffer = q_new<DX11Buffer>(EBufferType::Index, mUsage, desc.indexCount, getIndexSize());
    mBufferDeleter = &DeleteBuffer;
}

ID3D11Buffer *DX11IndexBuffer::getBuffer() const {
    return static_cast<DX11Buffer *>(mBuffer)->getBuffer();
}
