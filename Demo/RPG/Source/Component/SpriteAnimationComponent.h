#pragma once

#include "RPGPrerequisites.h"
#include "Scene/Component.h"

class SpriteAnimationComponent : public Component {
private:
    class SpriteRendererComponent *mSpriteRenderer;

public:
    void onStart() override;
};
