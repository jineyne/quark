#include "Vector4.h"

FVector4 FVector4::Max(const FVector4 &a, const FVector4 &b) {
    return (a.lengthSquared() > b.lengthSquared()) ? a : b;
}

FVector4 FVector4::Min(const FVector4 &a, const FVector4 &b) {
    return (a.lengthSquared() < b.lengthSquared()) ? a : b;
}

bool FVector4::operator==(const FVector4 &rhs) {
    return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

bool FVector4::operator!=(const FVector4 &rhs) {
    return !(*this == rhs);
}

FVector4 FVector4::operator+(const FVector4 &rhs) const {
    return FVector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

FVector4 FVector4::operator-(const FVector4 &rhs) const {
    return FVector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

FVector4 FVector4::operator*(const FVector4 &rhs) const {
    return FVector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
}

FVector4 FVector4::operator/(const FVector4 &rhs) const {
    return FVector4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
}

FVector4 &FVector4::operator+=(const FVector4 &rhs) {
    *this = *this + rhs;
    return *this;
}

FVector4 &FVector4::operator-=(const FVector4 &rhs) {
    *this = *this - rhs;
    return *this;
}

FVector4 &FVector4::operator*=(const FVector4 &rhs) {
    *this = *this * rhs;
    return *this;
}

FVector4 &FVector4::operator/=(const FVector4 &rhs) {
    *this = *this / rhs;
    return *this;
}

float FVector4::operator[](uint32_t i) const {
    assert(i < 4);

    return *(&x + i);
}

float &FVector4::operator[](uint32_t i) {
    assert(i < 4);

    return *(&x + i);
}

FVector4 &FVector4::clamp(FVector4 min, FVector4 max) {
    x = std::clamp(x, min.x, max.x);
    y = std::clamp(y, min.y, max.y);
    z = std::clamp(z, min.z, max.z);
    w = std::clamp(w, min.w, max.w);
    return *this;
}

float FVector4::distance(const FVector4 &other) const {
    return std::sqrt((distanceSquared(other)));
}

float FVector4::distanceSquared(const FVector4 &other) const {
    return (std::pow(x - other.x, 2) + std::pow(y - other.y, 2) + std::pow(z - other.z, 2) + std::pow(w - other.w, 2));
}

float FVector4::dot(const FVector4 &other) const {
    return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
}

float FVector4::length() const {
    return std::sqrt(lengthSquared());
}

float FVector4::lengthSquared() const {
    return (x * x) + (y * y) + (z * z) + (w * w);
}

void FVector4::normalize() {
    *this = normalized();
}

FVector4 FVector4::normalized() const {
    float factor = lengthSquared();
    if (factor == 0) {
        factor = 1.0f;
    } else {
        factor = 1.0f / std::sqrt(factor);
    }

    return FVector4(x * factor, y * factor, z * factor, 0.0f);
}

bool FVector4::equals(const FVector4 &other) const {
    return x == other.x && y == other.y && z == other.z && w == other.w;
}
