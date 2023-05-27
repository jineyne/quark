#include "VertexData.h"

void VertexData::setBuffer(uint32_t index, VertexBuffer *buffer) {
    mVertexBuffers[index] = buffer;

    recalculateMaxIndex();
}

VertexBuffer *VertexData::getBuffer(uint32_t index) const {
    auto it = mVertexBuffers.find(index);
    if (it != nullptr) {
        return *it;
    }

    return nullptr;
}

bool VertexData::isBufferBound(uint32_t index) const {
    auto it = mVertexBuffers.find(index);
    if (it != nullptr) {
        if ((*it) != nullptr) {
            return true;
        }
    }

    return false;
}

void VertexData::recalculateMaxIndex() {
    mMaxBufferIdx = 0;
    for (auto &entry : mVertexBuffers) {
        mMaxBufferIdx = std::max(entry.key, mMaxBufferIdx);
    }
}
