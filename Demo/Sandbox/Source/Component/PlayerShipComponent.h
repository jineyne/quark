#pragma once

#include "Prerequisites.h"
#include "Scene/Component.h"
#include "ShipAIComponent.h"
#include "PlayerShipComponent.g.h"

QCLASS()
class PlayerShipComponent : public ShipAIComponent {
    GENERATED_BODY()

public:
    TEvent<void()> Destroyed;

public:
    void onDestroy() override;
};
