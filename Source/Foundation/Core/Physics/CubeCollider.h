#pragma once

#include "CorePrerequisites.h"
#include "Collider.h"

class DLL_EXPORT CubeCollider : public Collider {
private:
    Vector3 mHalfSize;

public:
    CollisionPoints testCollision(const Transform *transform, const Collider *collider,
                                  const Transform *colliderTransform) const override;

    CollisionPoints testCollision(const Transform *transform, const CubeCollider *cube,
                                  const Transform *cubeTransform) const override;

    CollisionPoints testCollision(const Transform *transform, const SphereCollider *sphere,
                                  const Transform *sphereTransform) const override;

    CollisionPoints testCollision(const Transform *transform, const PlaneCollider *plane,
                                  const Transform *planeTransform) const override;

    AABB generateAABB() const override;

    const Vector3 &getHalfSize() const;
    void setHalfSize(const Vector3 &halfSize);
};
