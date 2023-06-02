#include "CubeCollider.h"

#include "PlaneCollider.h"
#include "SphereCollider.h"

CollisionPoints CubeCollider::testCollision(const Transform *transform, const Collider *collider,
                                            const Transform *colliderTransform) const {
    return collider->testCollision(colliderTransform, this, transform);
}

CollisionPoints CubeCollider::testCollision(const Transform *transform, const CubeCollider *cube,
                                            const Transform *cubeTransform) const {
    CollisionPoints points{};

    // Calculate the difference in position between the two cubes
    Vector3 diff = transform->getPosition() - cubeTransform->getPosition();

    // Calculate the total halfSize (sum of the halfSizes of the two cubes)
    Vector3 totalHalfSize = getHalfSize() + cube->getHalfSize();

    // If the absolute value of the difference in any dimension is greater than the total halfSize in that dimension,
    // then the cubes are not colliding
    if (std::abs(diff.x) > totalHalfSize.x || std::abs(diff.y) > totalHalfSize.y || std::abs(diff.z) > totalHalfSize.z) {
        points.hasCollision = false;
    } else {
        // The cubes are colliding
        points.hasCollision = true;

        // The point of collision is halfway between the cube positions
        points.a = transform->getPosition() + diff / 2;
        points.b = cubeTransform->getPosition() - diff / 2;

        // The normal is the normalized difference in position
        points.normal = diff.normalized();

        // The depth is the amount by which the total halfSize exceeds the absolute difference in position
        points.depth = (totalHalfSize - Vector3(std::abs(diff.x), std::abs(diff.y), std::abs(diff.z))).length();
    }

    return points;
}

CollisionPoints CubeCollider::testCollision(const Transform *transform, const SphereCollider *sphere,
                                            const Transform *sphereTransform) const {
    return sphere->testCollision(sphereTransform, this, transform);
}

CollisionPoints CubeCollider::testCollision(const Transform *transform, const PlaneCollider *plane,
                                            const Transform *planeTransform) const {
    return CollisionPoints();
}

AABB CubeCollider::generateAABB() const {
    auto center = getTransform()->getPosition();
    auto scaledHalfSize = getHalfSize();

    return AABB(center - scaledHalfSize, center + scaledHalfSize);
}

const Vector3 &CubeCollider::getHalfSize() const {
    return mHalfSize;
}

void CubeCollider::setHalfSize(const Vector3 &halfSize) {
    mHalfSize = halfSize;
    setDirty();
}
