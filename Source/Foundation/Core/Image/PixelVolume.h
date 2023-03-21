#pragma once

#include "CorePrerequisites.h"

class DLL_EXPORT FPixelVolume {
public:
    uint32_t left = 0;
    uint32_t top = 0;
    uint32_t right = 1;
    uint32_t bottom = 1;
    uint32_t front = 0;
    uint32_t back = 1;

public:
    FPixelVolume() = default;

    FPixelVolume(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom)
            : left(left), top(top), right(right), bottom(bottom) {
        assert(right >= left && bottom >= top && back >= front);
    }

    FPixelVolume(uint32_t left, uint32_t top, uint32_t front, uint32_t right, uint32_t bottom, uint32_t back)
            : left(left), top(top), right(right), bottom(bottom), front(front), back(back) {
        assert(right >= left && bottom >= top && back >= front);
    }

    bool contains(const FPixelVolume &volume) const {
        return (volume.left >= left && volume.top >= top && volume.front >= front &&
                volume.right <= right && volume.bottom <= bottom && volume.back <= back);
    }

    uint32_t getWidth() const { return right - left; }
    uint32_t getHeight() const { return bottom - top; }
    uint32_t getDepth() const { return back - front; }
};