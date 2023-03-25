#pragma once

#include "CorePrerequisites.h"
#include "VertexBuffer.h"

class DLL_EXPORT FVertexData {
public:
    FVertexDeclaration *vertexDeclaration;
    uint32_t vertexCount = 0;

private:
    TMap<uint32_t, FVertexBuffer *> mVertexBuffers;

    uint32_t mMaxBufferIdx = 0;

public:
    FVertexData() = default;
    ~FVertexData() = default;

public:
    void setBuffer(uint32_t index, FVertexBuffer *buffer);
    FVertexBuffer *getBuffer(uint32_t index) const;
    bool isBufferBound(uint32_t index) const;

    const TMap<uint32_t, FVertexBuffer *> &getBuffers() const { return mVertexBuffers; }
    uint32_t getBufferCount() const { return static_cast<uint32_t>(mVertexBuffers.length()); }
    uint32_t getMaxBufferIndex() const { return mMaxBufferIdx; }

private:
    void recalculateMaxIndex();
};