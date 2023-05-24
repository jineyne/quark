#include "Vector3.h"

FVector3 FVector3::ZeroVector = FVector3();
FVector3 FVector3::Forward = FVector3(0, 0, 1);
FVector3 FVector3::Right = FVector3(1, 0, 0);
FVector3 FVector3::Up = FVector3(0, 1, 0);

FVector3::FVector3() : x(0), y(0), z(0) {}
FVector3::FVector3(float x, float y, float z) : x(x), y(y), z(z) {}

float FVector3::Dot(const FVector3 &a, const FVector3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

FVector3 FVector3::Lerp(const FVector3 &start, const FVector3 &end, float t) {
    return start * (1.0f - t) + end * t;
}

FVector3 FVector3::Min(const FVector3 &a, const FVector3 &b) {
    return FVector3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

FVector3 FVector3::Max(const FVector3 &a, const FVector3 &b) {
    return FVector3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

FVector3 FVector3::Cross(const FVector3 &a, const FVector3 &b) {
    return FVector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

float FVector3::Distance(const FVector3 &a, const FVector3 &b) {
    return (b - a).length();
}

float FVector3::DistanceSquared(const FVector3 &a, const FVector3 &b) {
    return (b - a).dot(b - a);
}

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

float FVector3::dot(const FVector3 &other) const {
    return FVector3::Dot(*this, other);
}

FVector3 FVector3::cross(const FVector3 &other) const {
    return Cross(*this, other);
}

float FVector3::length() const {
    return sqrt(lengthSquared());
}

float FVector3::lengthSquared() const {
    return x * x + y * y + z * z;
}

FVector3 FVector3::normalized() const {
    float mag = length();
    if (mag > 0.0f) {
        return *this / mag;
    }
    return FVector3();
}

float FVector3::angle(const FVector3 &other) const {
    float d = dot(other);
    float mag = length() * other.length();
    if (mag > 0.0f) {
        return acos(d / mag);
    }
    return 0.0f;
}

FVector3 FVector3::rotate(float angle, const FVector3 &axis) const {
    float cosA = cos(angle);
    float sinA = sin(angle);
    FVector3 rotated(x * (cosA + (1 - cosA) * axis.x * axis.x) + y * ((1 - cosA) * axis.x * axis.y - sinA * axis.z) + z * ((1 - cosA) * axis.x * axis.z + sinA * axis.y),
                     x * ((1 - cosA) * axis.x * axis.y + sinA * axis.z) + y * (cosA + (1 - cosA) * axis.y * axis.y) + z * ((1 - cosA) * axis.y * axis.z - sinA * axis.x),
                     x * ((1 - cosA) * axis.x * axis.z - sinA * axis.y) + y * ((1 - cosA) * axis.y * axis.z + sinA * axis.x) + z * (cosA + (1 - cosA) * axis.z * axis.z));
    return rotated;
}

float FVector3::distance(const FVector3 &other) const {
    return Distance(*this, other);
}
