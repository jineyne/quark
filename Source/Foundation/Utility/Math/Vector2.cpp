//
// Created by jiney on 2023-03-09.
//

#include "Vector2.h"

Vector2 Vector2::Lerp(const Vector2 &start, const Vector2 &end, float t) {
    return start * (1.0f - t) + end * t;
}

Vector2 Vector2::Min(const Vector2 &a, const Vector2 &b) {
    return Vector2(std::min(a.x, b.x), std::min(a.y, b.y));
}

Vector2 Vector2::Max(const Vector2 &a, const Vector2 &b) {
    return Vector2(std::max(a.x, b.x), std::max(a.y, b.y));
}

float Vector2::Distance(const Vector2 &a, const Vector2 &b) {
    return (b - a).length();
}

Vector2 Vector2::operator+(const Vector2 &other) const { return Vector2(x + other.x, y + other.y); }

Vector2 Vector2::operator-(const Vector2 &other) const { return Vector2(x - other.x, y - other.y); }

Vector2 Vector2::operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }

Vector2 Vector2::operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }

bool Vector2::operator==(const Vector2 &other) const { return x == other.x && y == other.y; }

bool Vector2::operator!=(const Vector2 &other) const { return !(*this == other); }

float Vector2::dot(const Vector2 &other) const {
    return x * other.x + y * other.y;
}

float Vector2::cross(const Vector2 &other) const {
    return x * other.y - y * other.x;
}

float Vector2::length() const {
    return sqrt(x * x + y * y);
}

Vector2 Vector2::normalized() const {
    float mag = length();
    if (mag > 0.0f) {
        return *this / mag;
    }
    return Vector2();
}

float Vector2::angle(const Vector2 &other) const {
    float d = dot(other);
    float mag = length() * other.length();
    if (mag > 0.0f) {
        return acos(d / mag);
    }
    return 0.0f;
}

Vector2 Vector2::rotate(float angle) const {
    float cosA = cos(angle);
    float sinA = sin(angle);
    return Vector2(x * cosA - y * sinA, x * sinA + y * cosA);
}

float Vector2::distance(const Vector2 &other) const {
    return Distance(*this, other);
}
