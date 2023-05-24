#include "VertexData.h"

void FVertexData::setBuffer(uint32_t index, FVertexBuffer *buffer) {
    mVertexBuffers[index] = buffer;

    recalculateMaxIndex();
}

FVertexBuffer *FVertexData::getBuffer(uint32_t index) const {
    auto it = mVertexBuffers.find(index);
    if (it != nullptr) {
        return *it;
    }

    return nullptr;
}

bool FVertexData::isBufferBound(uint32_t index) const {
    auto it = mVertexBuffers.find(index);
    if (it != nullptr) {
        if ((*it) != nullptr) {
            return true;
        }
    }

    return false;
}

void FVertexData::recalculateMaxIndex() {
    mMaxBufferIdx = 0;
    for (auto &entry : mVertexBuffers) {
        mMaxBufferIdx = std::max(entry.key, mMaxBufferIdx);
    }
}
