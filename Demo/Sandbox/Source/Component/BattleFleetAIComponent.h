#pragma once

#include "Prerequisites.h"
#include "Scene/Component.h"
#include "ShipAIComponent.h"
#include "BattleFleetAIComponent.g.h"

QCLASS()
class BattleFleetAIComponent : public ShipAIComponent {
    GENERATED_BODY()

public:
    void spawnFighter();
};