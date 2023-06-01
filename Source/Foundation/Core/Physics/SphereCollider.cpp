#include "SphereCollider.h"

#include "CubeCollider.h"
#include "PlaneCollider.h"

CollisionPoints SphereCollider::testCollision(const Transform *transform, const Collider *collider,
                                              const Transform *colliderTransform) const {
    return collider->testCollision(colliderTransform, this, transform);
}

CollisionPoints SphereCollider::testCollision(const Transform *transform, const struct CubeCollider *cube,
                                              const Transform *cubeTransform) const {
    CollisionPoints point {};

    // Calculate the closest point in the cube to the center of the sphere
    Vector3 closestPointInCube = Vector3::Clamp(transform->getPosition(),
            cubeTransform->getPosition() - cube->getHalfSize(),
            cubeTransform->getPosition() + cube->getHalfSize()
    );

    // Calculate the vector from the center of the sphere to the closest point in the cube
    Vector3 sphereToCube = closestPointInCube - transform->getPosition();

    float distanceSquared = sphereToCube.lengthSquared();
    float radiusSquared = getRadius() * getRadius();

    // If the distance squared is less than the sphere's radius squared, they are colliding
    if (distanceSquared < radiusSquared) {
        float distance = std::sqrt(distanceSquared);

        point.hasCollision = true;
        point.a = closestPointInCube; // Closest point on cube
        point.b = transform->getPosition() + (sphereToCube / distance) * getRadius(); // Point on sphere
        point.normal = sphereToCube.normalized();
        point.depth = getRadius() - distance;
    } else {
        point.hasCollision = false;
    }

    return point;
}

CollisionPoints SphereCollider::testCollision(const Transform *transform, const SphereCollider *sphere,
                                              const Transform *sphereTransform) const {
    CollisionPoints points{};

    // this, transform, sphere, sphereTransform
    Vector3 a = getCenter() + transform->getPosition();
    Vector3 b = sphere->getCenter() + sphereTransform->getPosition();

    float ar = getRadius();
    float br = sphere->getRadius();

    Vector3 AtoB = b - a;
    Vector3 BtoA = a - b;

    if (AtoB.length() > ar + br) {
        return points;
    }

    points.a += AtoB.normalized() * ar;
    points.b += BtoA.normalized() * br;
    points.normal = AtoB.normalized();
    points.depth = AtoB.length();
    points.hasCollision = true;

    return points;
}

CollisionPoints SphereCollider::testCollision(const Transform *transform, const PlaneCollider *plane,
                                              const Transform *planeTransform) const {
    CollisionPoints points{};

    Vector3 a = getCenter() + transform->getPosition();
    // Vector3 b = plane->getPlane().P

    // TODO: 귀찮..
    return points;
}

AABB SphereCollider::generateAABB() const {
    Vector3 center = getCenter();
    Vector3 radius = Vector3(mRadius, mRadius, mRadius);
    return AABB(center - radius, center + radius);
}

void SphereCollider::setRadius(float radius) {
    mRadius = radius;
}

float SphereCollider::getRadius() const {
    return mRadius;
}