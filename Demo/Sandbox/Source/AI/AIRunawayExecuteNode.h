#pragma once

#include "Sandbox.h"
#include "AI/AIExecuteNode.h"

class AIRunawayExecuteNode : public AIExecuteNode {
private:
    float mAcceptanceRadius = 5.0f;
    float mSpeed = 1.0f;

public:
    EAIStatus execute() override;

    float getAcceptanceRadius() const;
    void setAcceptanceRadius(float acceptanceRadius);

    float getSpeed() const;
    void setSpeed(float speed);
};