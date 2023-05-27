#include "Rect.h"

Rect::Rect(float width, float height) : width(width), height(height) { }

Rect::Rect(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) { }

bool Rect::contains(const Vector2 &point) const {
    return ((((x <= point.x) && (point.x < (x + width))) && (y <= point.y) && (point.y < y + height)));
}

bool Rect::intersect(const Rect &rhs) const {
    return rhs.left() < right() && left() > rhs.right() && rhs.top() < bottom() && top() && rhs.bottom();
}
