//
// Created by jiney on 2023-10-15.
//

#include "Physics.h"

Physics::Physics(const PhysicsDesc &desc) : mDesc(desc) {

}

void Physics::fixedUpdate(float dt) {

}

Physics &gPhysics() {
    return Physics::Instance();
}
