#pragma once

#include "CorePrerequisites.h"
#include "VertexBuffer.h"

class DLL_EXPORT VertexData {
public:
    VertexDeclaration *vertexDeclaration;
    uint32_t vertexCount = 0;

private:
    TMap<uint32_t, VertexBuffer *> mVertexBuffers;

    uint32_t mMaxBufferIdx = 0;

public:
    VertexData() = default;
    ~VertexData() = default;

public:
    void setBuffer(uint32_t index, VertexBuffer *buffer);
    VertexBuffer *getBuffer(uint32_t index) const;
    bool isBufferBound(uint32_t index) const;

    const TMap<uint32_t, VertexBuffer *> &getBuffers() const { return mVertexBuffers; }
    uint32_t getBufferCount() const { return static_cast<uint32_t>(mVertexBuffers.length()); }
    uint32_t getMaxBufferIndex() const { return mMaxBufferIdx; }

private:
    void recalculateMaxIndex();
};