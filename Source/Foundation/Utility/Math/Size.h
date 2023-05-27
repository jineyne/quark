#pragma once

#include "UtilityCore.h"
#include "Size.g.h"

QSTRUCT()
struct DLL_EXPORT Size {
    GENERATED_BODY()
public:
    static Size ZeroSize;

    QPROPERTY()
    float width = 0.f;

    QPROPERTY()
    float height = 0.f;

public:
    Size() = default;
    Size(float width, float height);

public:
    bool operator==(const Size &rhs) const;
    bool operator!=(const Size &rhs) const;
    bool operator<(const Size &rhs) const;
    bool operator>(const Size &rhs) const;
    bool operator<=(const Size &rhs) const;
    bool operator>=(const Size &rhs) const;

    Size operator+(const Size &rhs) const;
    Size operator+(float value) const;
    Size operator-(const Size &rhs) const;
    Size operator-(float value) const;
    Size operator*(const Size &rhs) const;
    Size operator*(float value) const;
    Size operator/(const Size &rhs) const;
    Size operator/(float value) const;
};
