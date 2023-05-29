#pragma once

#include "Prerequisites.h"
#include "Scene/Component.h"
#include "BulletComponent.g.h"

QCLASS()
class BulletComponent : public Component {
    GENERATED_BODY()

private:
    QPROPERTY()
    float mLifeTime = 10;

public:
    void onUpdate() override;
};