#include "Size.h"

Size Size::ZeroSize = Size(0, 0);

Size::Size(float width, float height) : width(width), height(height) { }

bool Size::operator==(const Size &rhs) const {
    return width == rhs.width && height == rhs.height;
}

bool Size::operator!=(const Size &rhs) const {
    return !(rhs == *this);
}

bool Size::operator<(const Size &rhs) const {
    if (width < rhs.width) {
        return true;
    } else if (rhs.width < width) {
        return false;
    }

    return height < rhs.height;
}

bool Size::operator>(const Size &rhs) const {
    return rhs < *this;
}

bool Size::operator<=(const Size &rhs) const {
    return !(rhs < *this);
}

bool Size::operator>=(const Size &rhs) const {
    return !(*this < rhs);
}


Size Size::operator+(const Size &rhs) const {
    return Size(width + rhs.width, height + rhs.height);
}


Size Size::operator+(float value) const {
    return Size(width + value, height + value);
}


Size Size::operator-(const Size &rhs) const {
    return Size(width - rhs.width, height - rhs.height);
}


Size Size::operator-(float value) const {
    return Size(width - value, height - value);
}


Size Size::operator*(const Size &rhs) const {
    return Size(width * rhs.width, height * rhs.height);
}


Size Size::operator*(float value) const {
    return Size(width * value, height * value);
}


Size Size::operator/(const Size &rhs) const {
    return Size(width / rhs.width, height / rhs.height);
}


Size Size::operator/(float value) const {
    return Size(width / value, height / value);
}