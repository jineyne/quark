#pragma once

#include "UtilityCore.h"

QSTRUCT()
struct DLL_EXPORT FSize {
public:
    static FSize ZeroSize;

    float width = 0.f;

    float height = 0.f;

public:
    FSize() = default;

    FSize(float width, float height);

public:
    bool operator==(const FSize &rhs) const;
    bool operator!=(const FSize &rhs) const;
    bool operator<(const FSize &rhs) const;
    bool operator>(const FSize &rhs) const;
    bool operator<=(const FSize &rhs) const;
    bool operator>=(const FSize &rhs) const;

    FSize operator+(const FSize &rhs) const;
    FSize operator+(float value) const;
    FSize operator-(const FSize &rhs) const;
    FSize operator-(float value) const;
    FSize operator*(const FSize &rhs) const;
    FSize operator*(float value) const;
    FSize operator/(const FSize &rhs) const;
    FSize operator/(float value) const;
};
