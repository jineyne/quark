#pragma once

#include "UtilityCore.h"
#include "Vector2.h"
#include "Size.h"
#include "Rect.g.h"

QSTRUCT()
struct DLL_EXPORT Rect {
    GENERATED_BODY()
public:
    QPROPERTY()
    float x = 0.f;

    QPROPERTY()
    float y = 0.f;

    QPROPERTY()
    float width = 0.f;

    QPROPERTY()
    float height = 0.f;

public:
    Rect() = default;

    Rect(float width, float height);

    Rect(float x, float y, float width, float height);

public:
    bool contains(const Vector2 &point) const;

    bool intersect(const Rect &rhs) const;

    Vector2 point() const { return Vector2(x, y); }

    Size size() const { return Size(width, height); }

    float left() const { return x; }

    float right() const { return x + width; }

    float top() const { return y + height; }

    float bottom() const { return y; }
};
