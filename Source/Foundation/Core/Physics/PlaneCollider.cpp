#include "PlaneCollider.h"

#include "CubeCollider.h"
#include "SphereCollider.h"

CollisionPoints PlaneCollider::testCollision(const Transform *transform, const Collider *collider,
                                             const Transform *colliderTransform) const {
    return collider->testCollision(colliderTransform, this, transform);
}

CollisionPoints PlaneCollider::testCollision(const Transform *transform, const struct CubeCollider *cube,
                                             const Transform *cubeTransform) const {
    return cube->testCollision(cubeTransform, this, transform);
}

CollisionPoints PlaneCollider::testCollision(const Transform *transform, const SphereCollider *sphere,
                                             const Transform *sphereTransform) const {
    return sphere->testCollision(sphereTransform, this, transform);
}

CollisionPoints PlaneCollider::testCollision(const Transform *transform, const PlaneCollider *plane,
                                             const Transform *planeTransform) const {
    return {}; // No plane v plane
}
