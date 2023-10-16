//
// Created by jiney on 2023-10-16.
//

#include "SpriteAnimationComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "Scene/Actor.h"

void SpriteAnimationComponent::onStart() {
    Component::onStart();

    mSpriteRenderer = getOwner()->getComponent<SpriteRendererComponent>();
}
