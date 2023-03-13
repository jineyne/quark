//
// Created by jiney on 2023-03-09.
//

#include "Vector2.h"

FVector2 FVector2::Lerp(const FVector2 &start, const FVector2 &end, float t) {
    return start * (1.0f - t) + end * t;
}

FVector2 FVector2::Min(const FVector2 &a, const FVector2 &b) {
    return FVector2(std::min(a.x, b.x), std::min(a.y, b.y));
}

FVector2 FVector2::Max(const FVector2 &a, const FVector2 &b) {
    return FVector2(std::max(a.x, b.x), std::max(a.y, b.y));
}

float FVector2::Distance(const FVector2 &a, const FVector2 &b) {
    return (b - a).length();
}

FVector2 FVector2::operator+(const FVector2 &other) const { return FVector2(x + other.x, y + other.y); }

FVector2 FVector2::operator-(const FVector2 &other) const { return FVector2(x - other.x, y - other.y); }

FVector2 FVector2::operator*(float scalar) const { return FVector2(x * scalar, y * scalar); }

FVector2 FVector2::operator/(float scalar) const { return FVector2(x / scalar, y / scalar); }

bool FVector2::operator==(const FVector2 &other) const { return x == other.x && y == other.y; }

bool FVector2::operator!=(const FVector2 &other) const { return !(*this == other); }

float FVector2::dot(const FVector2 &other) const {
    return x * other.x + y * other.y;
}

float FVector2::cross(const FVector2 &other) const {
    return x * other.y - y * other.x;
}

float FVector2::length() const {
    return sqrt(x * x + y * y);
}

FVector2 FVector2::normalized() const {
    float mag = length();
    if (mag > 0.0f) {
        return *this / mag;
    }
    return FVector2();
}

float FVector2::angle(const FVector2 &other) const {
    float d = dot(other);
    float mag = length() * other.length();
    if (mag > 0.0f) {
        return acos(d / mag);
    }
    return 0.0f;
}

FVector2 FVector2::rotate(float angle) const {
    float cosA = cos(angle);
    float sinA = sin(angle);
    return FVector2(x * cosA - y * sinA, x * sinA + y * cosA);
}

float FVector2::distance(const FVector2 &other) const {
    return Distance(*this, other);
}
