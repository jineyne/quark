#pragma once

#include "CorePrerequisites.h"
#include "Physics/BoxCollider2D.h"
#include "Collider2DComponent.h"
#include "BoxCollider2DComponent.g.h"

QCLASS()
class DLL_EXPORT BoxCollider2DComponent : public Collider2DComponent {
    GENERATED_BODY();

private:
    Size mSize = Size(0, 0);

public:
    void onCreate() override;
    void onDestroy() override;

    void onFixedUpdate() override;

    void onTransformChanged(const ETransformChangedFlags &flags) override;

    const Size &getSize() const;
    void setSize(const Size &size);

    BoxCollider2D *_internal() const;
};
