//
// Created by jiney on 2023-05-29.
//

#include "AABB.h"

AABB::AABB(const Vector3 min, const Vector3 max) : mMinPoint(min), mMaxPoint(max) { }

AABB AABB::Union(const AABB &a, const AABB &b) {
    return AABB(Vector3::Min(a.getMinPoint(), b.getMinPoint()), Vector3::Max(a.getMaxPoint(), b.getMaxPoint()));
}

Vector3 AABB::getCenter() const {
    return (mMinPoint + mMaxPoint) * 0.5f;
}

Vector3 AABB::getSize() const {
    return mMaxPoint - mMinPoint;
}

float AABB::getArea() const {
    Vector3 d = mMaxPoint - mMinPoint;
    return 2.0f * (d.x * d.y + d.y * d.z + d.z * d.x);
}

bool AABB::contains(const Vector3 &point) const {
    return (point.x >= mMinPoint.x && point.x <= mMaxPoint.x &&
            point.y >= mMinPoint.y && point.y <= mMaxPoint.y &&
            point.z >= mMinPoint.z && point.z <= mMaxPoint.z);
}

bool AABB::intersects(const AABB &other) const {
    return (mMinPoint.x <= other.mMaxPoint.x && mMaxPoint.x >= other.mMinPoint.x) &&
           (mMinPoint.y <= other.mMaxPoint.y && mMaxPoint.y >= other.mMinPoint.y) &&
           (mMinPoint.z <= other.mMaxPoint.z && mMaxPoint.z >= other.mMinPoint.z);
}

const Vector3 &AABB::getMinPoint() const {
    return mMinPoint;
}

void AABB::setMinPoint(const Vector3 &minPoint) {
    mMinPoint = minPoint;
}

const Vector3 &AABB::getMaxPoint() const {
    return mMaxPoint;
}

void AABB::setMaxPoint(const Vector3 &maxPoint) {
    mMaxPoint = maxPoint;
}
