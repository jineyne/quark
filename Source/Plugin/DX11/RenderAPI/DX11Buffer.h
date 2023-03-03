#pragma once

#include "DX11Prerequisites.h"

#include "RenderAPI/Buffer.h"

class DX11_EXPORT FDX11Buffer : public FBuffer {
private:
    D3D11_BUFFER_DESC mDesc;
    ID3D11Buffer *mBuffer;
    D3D11_BIND_FLAG mBufferType;

public:
    FDX11Buffer(D3D11_BIND_FLAG bufferType, uint32_t size, EBufferUsage usage);
    ~FDX11Buffer() override;

public:
    void writeData(uint32_t offset, uint32_t length, const void *src, EBufferWriteType flags) override;
    ID3D11Buffer *getBuffer() const { return mBuffer; }

protected:
    void *map(uint32_t offset, uint32_t length, const EGpuLockOptions &options) override;
    void unmap() override;
};
