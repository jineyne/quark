#include "PlayerShipComponent.h"

void PlayerShipComponent::onDestroy() {
    Component::onDestroy();

    if (Destroyed != nullptr) {
        Destroyed();
    }
};