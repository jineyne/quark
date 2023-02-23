#include "Vector3.h"

FVector3 FVector3::ZeroVector = FVector3();

FVector3::FVector3() : x(0), y(0), z(0) {}
FVector3::FVector3(float x, float y, float z) : x(x), y(y), z(z) {}

FVector3 FVector3::operator+(const FVector3 &rhs) const {
    return FVector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

FVector3 FVector3::operator-(const FVector3 &rhs) const {
    return FVector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

FVector3 FVector3::operator*(const FVector3 &rhs) const {
    return FVector3(x * rhs.x, y * rhs.y, z * rhs.z);
}

FVector3 FVector3::operator/(const FVector3 &rhs) const {
    return FVector3(x / rhs.x, y / rhs.y, z / rhs.z);
}

FVector3 FVector3::operator*(float v) const {
    return FVector3(x * v, y * v, z * v);
}

FVector3 FVector3::operator/(float v) const {
    return FVector3(x / v, y / v, z / v);
}

FVector3 &FVector3::operator+=(const FVector3 &rhs) {
    *this = *this + rhs;
    return *this;
}

FVector3 &FVector3::operator-=(const FVector3 &rhs) {
    *this = *this - rhs;
    return *this;
}

FVector3 &FVector3::operator*=(const FVector3 &rhs) {
    *this = *this * rhs;
    return *this;
}

FVector3 &FVector3::operator/=(const FVector3 &rhs) {
    *this = *this / rhs;
    return *this;
}

FVector3 FVector3::operator-() const {
    return FVector3(-x, -y, -z);
}

bool FVector3::operator==(const FVector3 &rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z;
}

bool FVector3::operator!=(const FVector3 &rhs) const {
    return !(*this == rhs);
}

float FVector3::dot(const FVector3 &v) const {
    return x * v.x + y * v.y + z * v.z;
}

FVector3 FVector3::cross(const FVector3 &v) const {
    return FVector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

float FVector3::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

void FVector3::normalize() {
    float len = length();
    if (len != 0) {
        x /= len;
        y /= len;
        z /= len;
    }
}
