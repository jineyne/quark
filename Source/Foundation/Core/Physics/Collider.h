#pragma once

#include "CorePrerequisites.h"

#include "Scene/Transform.h"
#include "Math/AABB.h"

struct DLL_EXPORT CollisionPoints {
    Vector3 a;
    Vector3 b;
    Vector3 normal;
    float depth = 0;
    bool hasCollision = false;
};

class DLL_EXPORT Collider {
public:
    TEvent<void(Collider *)> CollisionEnter;
    TEvent<void(Collider *)> CollisionStay;
    TEvent<void(Collider *)> CollisionExit;

private:
    Transform *mTransform;

    Vector3 mOffset;

    AABB mRegisteredBounds;

    bool bIsTrigger = true;
    bool bIsTriggerOld = true;

    bool bIsDirty = true;

    bool bIsActive = true;
    bool bIsActiveOld = true;

public:
    ~Collider();

public:
    void initialize();

    void fixedUpdate();

    void updateData(EActorDirtyFlags flags);

    bool isTrigger() const;
    void setIsTrigger(bool isTrigger);

    bool isActive() const;
    void setActive(bool isActive);

    bool isDirty() const;
    void setDirty(bool isDirty = true);

    Transform *getTransform() const;
    void setTransform(Transform *transform);

    const Vector3 &getOffset() const;
    void setOffset(const Vector3 &offset);

    Vector3 getCenter() const;

    virtual CollisionPoints testCollision(const Transform *transform, const Collider *collider,
                                          const Transform *colliderTransform) const = 0;

    virtual CollisionPoints testCollision(const Transform *transform, const class CubeCollider *cube,
                                          const Transform *cubeTransform) const = 0;

    virtual CollisionPoints testCollision(const Transform *transform, const class SphereCollider *sphere,
                                          const Transform *sphereTransform) const = 0;

    virtual CollisionPoints testCollision(const Transform *transform, const class PlaneCollider *plane,
                                          const Transform *planeTransform) const = 0;

    virtual AABB generateAABB() const = 0;

private:
    friend class Physics;

    bool isTriggerOld() const;
    void setIsTriggerOld(bool isTriggerOld);

    const AABB &getRegisteredBounds() const;
    void setRegisteredBounds(const AABB &registeredBounds);
};