#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/VertexBuffer.h"

class DX11_EXPORT FDX11VertexBuffer : public FVertexBuffer {
public:
    FDX11VertexBuffer(const FVertexBufferDesc &desc);
    ~FDX11VertexBuffer() = default;

public:
    ID3D11Buffer *getBuffer() const;
};