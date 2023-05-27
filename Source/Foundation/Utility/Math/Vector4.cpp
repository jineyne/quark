#include "Vector4.h"

Vector4 Vector4::Max(const Vector4 &a, const Vector4 &b) {
    return (a.lengthSquared() > b.lengthSquared()) ? a : b;
}

Vector4 Vector4::Min(const Vector4 &a, const Vector4 &b) {
    return (a.lengthSquared() < b.lengthSquared()) ? a : b;
}

bool Vector4::operator==(const Vector4 &rhs) {
    return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

bool Vector4::operator!=(const Vector4 &rhs) {
    return !(*this == rhs);
}

Vector4 Vector4::operator+(const Vector4 &rhs) const {
    return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

Vector4 Vector4::operator-(const Vector4 &rhs) const {
    return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

Vector4 Vector4::operator*(const Vector4 &rhs) const {
    return Vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
}

Vector4 Vector4::operator/(const Vector4 &rhs) const {
    return Vector4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
}

Vector4 &Vector4::operator+=(const Vector4 &rhs) {
    *this = *this + rhs;
    return *this;
}

Vector4 &Vector4::operator-=(const Vector4 &rhs) {
    *this = *this - rhs;
    return *this;
}

Vector4 &Vector4::operator*=(const Vector4 &rhs) {
    *this = *this * rhs;
    return *this;
}

Vector4 &Vector4::operator/=(const Vector4 &rhs) {
    *this = *this / rhs;
    return *this;
}

float Vector4::operator[](uint32_t i) const {
    assert(i < 4);

    return *(&x + i);
}

float &Vector4::operator[](uint32_t i) {
    assert(i < 4);

    return *(&x + i);
}

Vector4 &Vector4::clamp(Vector4 min, Vector4 max) {
    x = std::clamp(x, min.x, max.x);
    y = std::clamp(y, min.y, max.y);
    z = std::clamp(z, min.z, max.z);
    w = std::clamp(w, min.w, max.w);
    return *this;
}

float Vector4::distance(const Vector4 &other) const {
    return std::sqrt((distanceSquared(other)));
}

float Vector4::distanceSquared(const Vector4 &other) const {
    return (std::pow(x - other.x, 2) + std::pow(y - other.y, 2) + std::pow(z - other.z, 2) + std::pow(w - other.w, 2));
}

float Vector4::dot(const Vector4 &other) const {
    return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
}

float Vector4::length() const {
    return std::sqrt(lengthSquared());
}

float Vector4::lengthSquared() const {
    return (x * x) + (y * y) + (z * z) + (w * w);
}

void Vector4::normalize() {
    *this = normalized();
}

Vector4 Vector4::normalized() const {
    float factor = lengthSquared();
    if (factor == 0) {
        factor = 1.0f;
    } else {
        factor = 1.0f / std::sqrt(factor);
    }

    return Vector4(x * factor, y * factor, z * factor, 0.0f);
}

bool Vector4::equals(const Vector4 &other) const {
    return x == other.x && y == other.y && z == other.z && w == other.w;
}
