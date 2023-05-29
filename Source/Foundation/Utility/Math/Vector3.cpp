#include "Vector3.h"

Vector3 Vector3::ZeroVector = Vector3();
Vector3 Vector3::Forward = Vector3(0, 0, 1);
Vector3 Vector3::Right = Vector3(1, 0, 0);
Vector3 Vector3::Up = Vector3(0, 1, 0);

Vector3::Vector3() : x(0), y(0), z(0) {}
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

float Vector3::Dot(const Vector3 &a, const Vector3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Vector3::Clamp(const Vector3 &val, const Vector3 &min, const Vector3 &max) {
    return Min(Max(val, min), max);
}

Vector3 Vector3::Lerp(const Vector3 &start, const Vector3 &end, float t) {
    return start * (1.0f - t) + end * t;
}

Vector3 Vector3::Min(const Vector3 &a, const Vector3 &b) {
    return Vector3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

Vector3 Vector3::Max(const Vector3 &a, const Vector3 &b) {
    return Vector3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

Vector3 Vector3::Cross(const Vector3 &a, const Vector3 &b) {
    return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

float Vector3::Distance(const Vector3 &a, const Vector3 &b) {
    return (b - a).length();
}

float Vector3::DistanceSquared(const Vector3 &a, const Vector3 &b) {
    return (b - a).dot(b - a);
}

Vector3 Vector3::operator+(const Vector3 &rhs) const {
    return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector3 Vector3::operator-(const Vector3 &rhs) const {
    return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vector3 Vector3::operator*(const Vector3 &rhs) const {
    return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
}

Vector3 Vector3::operator/(const Vector3 &rhs) const {
    return Vector3(x / rhs.x, y / rhs.y, z / rhs.z);
}

Vector3 Vector3::operator*(float v) const {
    return Vector3(x * v, y * v, z * v);
}

Vector3 Vector3::operator/(float v) const {
    return Vector3(x / v, y / v, z / v);
}

Vector3 &Vector3::operator+=(const Vector3 &rhs) {
    *this = *this + rhs;
    return *this;
}

Vector3 &Vector3::operator-=(const Vector3 &rhs) {
    *this = *this - rhs;
    return *this;
}

Vector3 &Vector3::operator*=(const Vector3 &rhs) {
    *this = *this * rhs;
    return *this;
}

Vector3 &Vector3::operator/=(const Vector3 &rhs) {
    *this = *this / rhs;
    return *this;
}

Vector3 Vector3::operator-() const {
    return Vector3(-x, -y, -z);
}

bool Vector3::operator==(const Vector3 &rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z;
}

bool Vector3::operator!=(const Vector3 &rhs) const {
    return !(*this == rhs);
}

float Vector3::dot(const Vector3 &other) const {
    return Vector3::Dot(*this, other);
}

Vector3 Vector3::cross(const Vector3 &other) const {
    return Cross(*this, other);
}

float Vector3::length() const {
    return sqrt(lengthSquared());
}

float Vector3::lengthSquared() const {
    return x * x + y * y + z * z;
}

Vector3 Vector3::normalized() const {
    float mag = length();
    if (mag > 0.0f) {
        return *this / mag;
    }
    return Vector3();
}

float Vector3::angle(const Vector3 &other) const {
    float d = dot(other);
    float mag = length() * other.length();
    if (mag > 0.0f) {
        return acos(d / mag);
    }
    return 0.0f;
}

Vector3 Vector3::rotate(float angle, const Vector3 &axis) const {
    float cosA = cos(angle);
    float sinA = sin(angle);
    Vector3 rotated(x * (cosA + (1 - cosA) * axis.x * axis.x) + y * ((1 - cosA) * axis.x * axis.y - sinA * axis.z) + z * ((1 - cosA) * axis.x * axis.z + sinA * axis.y),
                     x * ((1 - cosA) * axis.x * axis.y + sinA * axis.z) + y * (cosA + (1 - cosA) * axis.y * axis.y) + z * ((1 - cosA) * axis.y * axis.z - sinA * axis.x),
                     x * ((1 - cosA) * axis.x * axis.z - sinA * axis.y) + y * ((1 - cosA) * axis.y * axis.z + sinA * axis.x) + z * (cosA + (1 - cosA) * axis.z * axis.z));
    return rotated;
}

float Vector3::distance(const Vector3 &other) const {
    return Distance(*this, other);
}
