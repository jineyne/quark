#pragma once

#include "Prerequisites.h"
#include "AI/AIExecuteNode.h"
#include "Misc/Random.h"
#include "AIMoveRandomPositionExecuteNode.g.h"

QCLASS()
class DLL_EXPORT AIMoveRandomPositionExecuteNode : public AIExecuteNode {
    GENERATED_BODY();

private:
    QPROPERTY()
    Vector3 mDestination;

    QPROPERTY()
    float mAcceptanceRadius = 5.0f;

    QPROPERTY()
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
