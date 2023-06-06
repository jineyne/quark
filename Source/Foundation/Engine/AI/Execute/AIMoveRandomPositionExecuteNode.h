#pragma once

#include "Prerequisites.h"
#include "AI/AIExecuteNode.h"

#include "Misc/Random.h"

class DLL_EXPORT AIMoveRandomPositionExecuteNode : public AIExecuteNode {
private:
    Vector3 mDestination;
    float mAcceptanceRadius = 5.0f;
    float mSpeed = 1.0f;
    Random random;

public:
    AIMoveRandomPositionExecuteNode();

    EAIStatus execute() override;

    const Vector3 &getDestination() const;
    void setDestination(const Vector3 &destination);

    float getAcceptanceRadius() const;
    void setAcceptanceRadius(float acceptanceRadius);

    float getSpeed() const;
    void setSpeed(float speed);

private:
    void setRandomDestination();
};
