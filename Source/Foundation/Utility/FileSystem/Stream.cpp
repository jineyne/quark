#include "Stream.h"

FStream::FStream(EStreamAccessMode access) : mAccess(access) {}
FStream::FStream(FString name, EStreamAccessMode access) : mName(std::move(name)), mAccess(access) {}

size_t FStream::readSwap(void *buf, size_t num) {
    size_t amount = read(buf, num);
    if (isByteSwapping()) {
        byteSwap(buf, num);
    }
    return amount;
}

size_t FStream::writeSwap(void *buf, size_t num) {
    if (isByteSwapping()) { byteSwap(buf, num); }
    size_t amount = write(buf, num);
    if (isByteSwapping()) { byteSwap(buf, num); }

    return amount;
}

void FStream::byteSwap(void *buf, size_t num) {
    uint8_t* ptr = (uint8_t*) buf;
    size_t top = num - 1;
    size_t bottom = 0;
    while (bottom < top) {
        std::swap(ptr[top--], ptr[bottom++]);
    }
}

bool FStream::isByteSwapping() const {
#if PLATFORM_LITTLE_ENDIAN
    return false;
#else
    // TODO:
    return false;
#endif
}
