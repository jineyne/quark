#pragma once

#include "Sandbox.h"
#include "Scene/Component.h"
#include "FollowTargetComponent.g.h"

QCLASS()
class FollowTargetComponent : public Component {
    GENERATED_BODY()
private:
    Actor *mTarget;
    Vector3 mOffset = Vector3(0, 0, 5);

public:
    void onFixedUpdate() override;

    Actor *getTarget() const;
    void setTarget(Actor *target);

    const Vector3 &getOffset() const;
    void setOffset(const Vector3 &offset);
};