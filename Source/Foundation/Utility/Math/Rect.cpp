#include "Rect.h"

FRect::FRect(float width, float height) : width(width), height(height) { }

FRect::FRect(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) { }

bool FRect::contains(const FVector2 &point) const {
    return ((((x <= point.x) && (point.x < (x + width))) && (y <= point.y) && (point.y < y + height)));
}

bool FRect::intersect(const FRect &rhs) const {
    return rhs.left() < right() && left() > rhs.right() && rhs.top() < bottom() && top() && rhs.bottom();
}
