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
    return FVector3(x * (v), y * (v), z * (v));
}

FVector3 FVector3::operator/(float v) const {
    return FVector3(x / (v), y / (v), z / (v));
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

FVector3 FVector3::operator-() {
    x = -x;
    y = -y;
    z = -z;

    return *this;
}

bool FVector3::operator==(const FVector3 &rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z;
}

bool FVector3::operator!=(const FVector3 &rhs) const {
    return !(*this == rhs);
}