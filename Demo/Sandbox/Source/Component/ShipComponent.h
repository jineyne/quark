#pragma once

#include "Prerequisites.h"
#include "Scene/Component.h"
#include "ShipComponent.g.h"

QCLASS()
class ShipComponent : public Component {
    GENERATED_BODY()

private:
    QPROPERTY()
    float mHealth = 10;

public:
    void onUpdate() override;
};