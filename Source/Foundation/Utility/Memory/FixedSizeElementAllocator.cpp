#include "FixedSizeElementAllocator.h"

FixedSizeElementAllocator::FixedSizeElementAllocator(FixedSizeElementAllocator &&rhs) noexcept
        : mElementSize(std::exchange(rhs.mElementSize, 0)) {}

FixedSizeElementAllocator::~FixedSizeElementAllocator() {
}

FixedSizeElementAllocator &FixedSizeElementAllocator::operator=(FixedSizeElementAllocator &&rhs) noexcept {
    if (this == &rhs) {
        return *this;
    }

    mElementSize = std::exchange(rhs.mElementSize, 0);

    return *this;
}

void FixedSizeElementAllocator::init(size_t elementSize) {
    mElementSize = elementSize;
}

void *FixedSizeElementAllocator::alloc(uint32_t amount) {
    return q_alloc(amount * mElementSize);
}

void FixedSizeElementAllocator::free(void *ptr) {
    q_free((uint8_t *) ptr);
}