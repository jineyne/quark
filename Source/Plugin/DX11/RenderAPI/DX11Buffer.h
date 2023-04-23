#pragma once

#include "DX11Prerequisites.h"

#include "RenderAPI/Buffer.h"

class DX11_EXPORT FDX11Buffer : public FBuffer {
private:
    D3D11_BUFFER_DESC mDesc;
    ID3D11Buffer *mBuffer;
    EBufferType mBufferType;

public:
    FDX11Buffer(EBufferType bufferType, EBufferUsage usage, uint32_t elementCount, uint32_t elementSize);
    ~FDX11Buffer() override;

public:
    void writeData(uint32_t offset, uint32_t length, const void *src, EBufferWriteType flags) override;
    ID3D11Buffer *getBuffer() const { return mBuffer; }

protected:
    void *map(uint32_t offset, uint32_t length, const EGpuLockOptions &options) override;
    void unmap() override;
};
