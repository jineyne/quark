#pragma once

#include "UtilityCore.h"
#include "Vector3.h"

class DLL_EXPORT AABB {
private:
    Vector3 mMinPoint;
    Vector3 mMaxPoint;

public:
    AABB() = default;
    AABB(const Vector3 min, const Vector3 max);

public:
    static AABB Union(const AABB &a, const AABB &b);

public:
    Vector3 getCenter() const;
    Vector3 getSize() const;

    float getArea() const;

    bool contains(const Vector3 &point) const;
    bool contains(const AABB &other) const;

    bool intersects(const AABB &other) const;

    const Vector3 &getMinPoint() const;
    void setMinPoint(const Vector3 &minPoint);

    const Vector3 &getMaxPoint() const;
    void setMaxPoint(const Vector3 &maxPoint);
};