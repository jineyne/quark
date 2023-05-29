#pragma once

#include "Prerequisites.h"
#include "Scene/Component.h"
#include "ShipComponent.h"
#include "FighterAIComponent.g.h"

QCLASS()
class FighterAIComponent : public ShipComponent {
    GENERATED_BODY()
private:
    int mTeam;

    float mHealth = 10;
    float mSpeed = 0;

    float mBulletInterval = 0;

    Actor *mPlayer;

public:
    void setTeam(int team);

    void fire();

    void onCreate() override;
    void onUpdate() override;
};