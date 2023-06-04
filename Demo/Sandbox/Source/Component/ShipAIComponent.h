#pragma once

#include "Prerequisites.h"
#include "Scene/Component.h"
#include "Physics/Collider.h"
#include "ShipAIComponent.g.h"

QCLASS()
class ShipAIComponent : public Component {
    GENERATED_BODY()

private:
    int mTeam;
    float mHealth = 10;

    ShipAIComponent *mParentAI;
    class BehaviourTreeComponent *mBehaviourTree = nullptr;

    TArray<ShipAIComponent *> mChildAIList;

public:
    void onCreate() override;

    void fire();

    void assignTo(ShipAIComponent *parent);
    void dissociate();

    int getTeam() const;
    void setTeam(int team);

    float getHealth() const;
    void setHealth(float health);

    BehaviourTreeComponent *getBehaviourTree() const;

protected:
    virtual void setupAI();

private:
    void addChild(ShipAIComponent *child);
    void removeChild(ShipAIComponent *child);

    void onShipCollisionEnter(Collider *other);
};