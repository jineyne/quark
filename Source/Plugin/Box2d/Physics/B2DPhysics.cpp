#include "B2DPhysics.h"

B2DPhysics::B2DPhysics(const PhysicsDesc &desc) : Physics(desc) {}

PhysicsScene *B2DPhysics::createPhysicsScene() {
    auto scene = q_new<B2DPhysicsScene>(this, getDesc());
    mScenes.add(scene);

    return scene;
}

void B2DPhysics::fixedUpdate(float dt) {
    Physics::fixedUpdate(dt);

    for (auto &scene : mScenes) {
        auto world = scene->getWorld();

        world->Step(dt, 6, 2);
    }
}

