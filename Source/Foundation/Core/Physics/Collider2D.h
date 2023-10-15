#pragma once

#include "CorePrerequisites.h"
#include "Scene/Actor.h"

enum class EPhysicsBodyType {
    Static,     //< 정적 몸체
    Kinematic,  //< 운동학적 몸체 시뮬레이션에 따라 속도에 따라 움직인다
    Dynamic     //< 동적 몸체, 힘에 의해 움직인다.
};

class DLL_EXPORT Collider2D {
public:
    TEvent<void(Collider2D *)> CollisionEnter;
    TEvent<void(Collider2D *)> CollisionExit;

private:
    Actor *mOwner;

public:
    virtual ~Collider2D() = default;

public:
    virtual void setPhysicsBodyType(EPhysicsBodyType type) = 0;
    virtual void setIsTrigger(bool isTrigger) = 0;
    virtual void setOffset(Vector2 offset) = 0;

    virtual void updateTransform(Vector2 position, FQuaternion rotation) = 0;

    virtual Vector2 getPosition() const = 0;
    virtual FQuaternion getRotation() const = 0;

    Actor *getOwner() const;
    void setOwner(Actor *owner);
};
