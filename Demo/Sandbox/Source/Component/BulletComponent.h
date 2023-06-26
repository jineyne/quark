#pragma once

#include "Sandbox.h"
#include "Scene/Component.h"
#include "Physics/Collider.h"
#include "BulletComponent.g.h"

QCLASS()
class BulletComponent : public Component {
    GENERATED_BODY()

private:
    QPROPERTY()
    float mLifeTime = 10;

    QPROPERTY()
    float mDamage = 1;

    QPROPERTY()
    int mTeam = 0;

public:
    void onCreate() override;
    void onUpdate() override;

    void onCollisionEnter(Collider *other);

    float getDamage() const;
    void setDamage(float damage);

    int getTeam() const;
    void setTeam(int team);
};