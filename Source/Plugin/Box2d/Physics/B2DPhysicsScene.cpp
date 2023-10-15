#include "B2DPhysicsScene.h"

#include "B2DPhysics.h"
#include "B2DBoxCollider2D.h"
#include "B2DSphereCollider2D.h"

B2DPhysicsScene::B2DPhysicsScene(B2DPhysics *physics, const PhysicsDesc &desc) : mPhysics(physics) {
    b2Vec2 gravity(desc.gravity.x, desc.gravity.y);

    mWorld = q_new<b2World>(gravity);
    mWorld->SetContactListener(this);
}

B2DPhysicsScene::~B2DPhysicsScene() {
    mWorld->SetContactListener(nullptr);
    q_delete(mWorld);
}

BoxCollider2D *B2DPhysicsScene::createBoxCollider2D(const Size &size, Transform *transform) {
    return q_new<B2DBoxCollider2D>(mPhysics, this, size, transform);
}

SphereCollider2D *B2DPhysicsScene::createSphereCollider2D(float radius, Transform *transform) {
    return q_new<B2DSphereCollider2D>(mPhysics, this, radius, transform);
}

void B2DPhysicsScene::BeginContact(b2Contact *contact) {
    b2ContactListener::BeginContact(contact);

    auto *aBody = contact->GetFixtureA()->GetBody();
    auto *bBody = contact->GetFixtureB()->GetBody();

    auto aCollider = (Collider2D *) aBody->GetUserData().pointer;
    auto bCollider = (Collider2D *) bBody->GetUserData().pointer;

    aCollider->CollisionEnter(bCollider);
    bCollider->CollisionEnter(aCollider);
}

void B2DPhysicsScene::EndContact(b2Contact *contact) {
    b2ContactListener::EndContact(contact);

    auto *aBody = contact->GetFixtureA()->GetBody();
    auto *bBody = contact->GetFixtureB()->GetBody();

    auto aCollider = (Collider2D *) aBody->GetUserData().pointer;
    auto bCollider = (Collider2D *) bBody->GetUserData().pointer;

    aCollider->CollisionExit(bCollider);
    bCollider->CollisionExit(aCollider);
}
