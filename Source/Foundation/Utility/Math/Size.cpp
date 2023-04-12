#include "Size.h"

FSize::FSize(float width, float height) : width(width), height(height) { }

bool FSize::operator==(const FSize &rhs) const {
    return width == rhs.width && height == rhs.height;
}

bool FSize::operator!=(const FSize &rhs) const {
    return !(rhs == *this);
}

bool FSize::operator<(const FSize &rhs) const {
    if (width < rhs.width) {
        return true;
    } else if (rhs.width < width) {
        return false;
    }

    return height < rhs.height;
}

bool FSize::operator>(const FSize &rhs) const {
    return rhs < *this;
}

bool FSize::operator<=(const FSize &rhs) const {
    return !(rhs < *this);
}

bool FSize::operator>=(const FSize &rhs) const {
    return !(*this < rhs);
}


FSize FSize::operator+(const FSize &rhs) const {
    return FSize(width + rhs.width, height + rhs.height);
}


FSize FSize::operator+(float value) const {
    return FSize(width + value, height + value);
}


FSize FSize::operator-(const FSize &rhs) const {
    return FSize(width - rhs.width, height - rhs.height);
}


FSize FSize::operator-(float value) const {
    return FSize(width - value, height - value);
}


FSize FSize::operator*(const FSize &rhs) const {
    return FSize(width * rhs.width, height * rhs.height);
}


FSize FSize::operator*(float value) const {
    return FSize(width * value, height * value);
}


FSize FSize::operator/(const FSize &rhs) const {
    return FSize(width / rhs.width, height / rhs.height);
}


FSize FSize::operator/(float value) const {
    return FSize(width / value, height / value);
}