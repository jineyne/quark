#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/VertexBuffer.h"

class DX11_EXPORT DX11VertexBuffer : public VertexBuffer {
public:
    DX11VertexBuffer(const VertexBufferDesc &desc);
    ~DX11VertexBuffer() = default;

public:
    ID3D11Buffer *getBuffer() const;
};