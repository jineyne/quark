#pragma once

#include "UtilityCore.h"
#include "Vector2.h"
#include "Size.h"

QSTRUCT()
struct DLL_EXPORT FRect {
private:
public:
    float x = 0.f;

    float y = 0.f;

    float width = 0.f;

    float height = 0.f;

public:
    FRect() = default;

    FRect(float width, float height);

    FRect(float x, float y, float width, float height);

public:
    bool contains(const FVector2 &point) const;

    bool intersect(const FRect &rhs) const;

    FVector2 point() const { return FVector2(x, y); }

    FSize size() const { return FSize(width, height); }

    float left() const { return x; }

    float right() const { return x + width; }

    float top() const { return y + height; }

    float bottom() const { return y; }
};
