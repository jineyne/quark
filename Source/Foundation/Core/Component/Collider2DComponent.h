#pragma once

#include "CorePrerequisites.h"
#include "Physics/Collider2D.h"
#include "Scene/Component.h"
#include "Collider2DComponent.g.h"

QCLASS()
class DLL_EXPORT Collider2DComponent : public Component {
    GENERATED_BODY()

public:
    TEvent<void(Collider2D *)> CollisionEnter;
    TEvent<void(Collider2D *)> CollisionExit;

private:
    EPhysicsBodyType mBodyType;
    Vector2 mOffset = Vector2(1, 1);
    bool bIsTrigger = false;

protected:
    Collider2D *mInternal;

public:
    void onCreate() override;

    const Vector2 &getOffset() const;
    void setOffset(const Vector2 &offset);

    bool isIsTrigger() const;
    void setIsTrigger(bool isTrigger);

    void setBodyType(EPhysicsBodyType type);
};

