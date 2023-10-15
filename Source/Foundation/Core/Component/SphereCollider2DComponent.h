#pragma once

#include "CorePrerequisites.h"
#include "Physics/SphereCollider2D.h"
#include "Collider2DComponent.h"
#include "SphereCollider2DComponent.g.h"

QCLASS()
class DLL_EXPORT SphereCollider2DComponent : public Collider2DComponent {
    GENERATED_BODY();

private:
    float mRadius = 0;

public:
    void onCreate() override;
    void onDestroy() override;

    void onFixedUpdate() override;

    void onTransformChanged(const ETransformChangedFlags &flags) override;

    float getRadius() const;

    void setRadius(float radius);

    SphereCollider2D *_internal() const;
};