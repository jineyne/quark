#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/IndexBuffer.h"

class DX11IndexBuffer : public IndexBuffer {
public:
    DX11IndexBuffer(const FIndexBufferDesc &desc);
    ~DX11IndexBuffer() = default;

public:
    ID3D11Buffer *getBuffer() const;
};