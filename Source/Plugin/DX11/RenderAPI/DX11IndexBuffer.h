#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/IndexBuffer.h"

class FDX11IndexBuffer : public FIndexBuffer {
public:
    FDX11IndexBuffer(const FIndexBufferDesc &desc);
    ~FDX11IndexBuffer() = default;

public:
    ID3D11Buffer *getBuffer() const;
};