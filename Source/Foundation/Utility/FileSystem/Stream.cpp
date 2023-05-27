#include "Stream.h"

Stream::Stream(EStreamAccessMode access) : mAccess(access) {}
Stream::Stream(String name, EStreamAccessMode access) : mName(std::move(name)), mAccess(access) {}

size_t Stream::readSwap(void *buf, size_t num) {
    size_t amount = read(buf, num);
    if (isByteSwapping()) {
        byteSwap(buf, num);
    }
    return amount;
}

size_t Stream::writeSwap(void *buf, size_t num) {
    if (isByteSwapping()) { byteSwap(buf, num); }
    size_t amount = write(buf, num);
    if (isByteSwapping()) { byteSwap(buf, num); }

    return amount;
}

void Stream::byteSwap(void *buf, size_t num) {
    uint8_t* ptr = (uint8_t*) buf;
    size_t top = num - 1;
    size_t bottom = 0;
    while (bottom < top) {
        std::swap(ptr[top--], ptr[bottom++]);
    }
}

bool Stream::isByteSwapping() const {
#if PLATFORM_LITTLE_ENDIAN
    return false;
#else
    // TODO:
    return false;
#endif
}
