#include "B2DBoxCollider2D.h"

#include <Scene/Transform.h>

#include "B2DPhysics.h"
#include "B2DPhysicsScene.h"
#include "B2DMapping.h"

B2DBoxCollider2D::B2DBoxCollider2D(B2DPhysics *physics, B2DPhysicsScene *scene, const Size &size, Transform *transform)
        : mPhysics(physics), mScene(scene), mSize(size) {
    setOwner(transform->getOwner());

    auto position = transform->getPosition();

    b2BodyDef bodyDef;
    bodyDef.type = B2DMapping::GetBodyType(getPhysicsBodyType());
    bodyDef.position.Set(position.x, position.y);
    bodyDef.userData.pointer = (uintptr_t) (Collider2D *) this;

    mBody = mScene->getWorld()->CreateBody(&bodyDef);
    b2PolygonShape boxShape;
    boxShape.SetAsBox(mSize.width / 2.0f, mSize.height / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    mFixture = mBody->CreateFixture(&fixtureDef);
}

B2DBoxCollider2D::~B2DBoxCollider2D() {
    mBody->DestroyFixture(mFixture);
    mScene->getWorld()->DestroyBody(mBody);
}

void B2DBoxCollider2D::setPhysicsBodyType(EPhysicsBodyType type) {
    BoxCollider2D::setPhysicsBodyType(type);

    mBody->SetType(B2DMapping::GetBodyType(type));
}

void B2DBoxCollider2D::setAwake(bool awake) {
    mBody->SetAwake(awake);
}

void B2DBoxCollider2D::setIsTrigger(bool isTrigger) {
    mBody->SetEnabled(isTrigger);
}

void B2DBoxCollider2D::setSize(Size size) {
    if (mSize == size) {
        return;
    }

    mSize = size;

    auto shape = (b2PolygonShape *) mFixture->GetShape();
    shape->SetAsBox(size.width / 2, size.height / 2);
}

void B2DBoxCollider2D::setOffset(Vector2 offset) {
    // do nothing
}

void B2DBoxCollider2D::updateTransform(Vector2 position, FQuaternion rotation) {
    mBody->SetTransform({position.x, position.y}, rotation.z);
    mBody->SetAwake(true);
}

b2Body *B2DBoxCollider2D::getBody() const {
    return mBody;
}

b2Fixture *B2DBoxCollider2D::getFixture() const {
    return mFixture;
}

Vector2 B2DBoxCollider2D::getPosition() const {
    auto pos = mBody->GetPosition();
    return {pos.x, pos.y};
}

FQuaternion B2DBoxCollider2D::getRotation() const {
    auto angle = mBody->GetAngle();
    return FQuaternion(Vector3::Forward, angle);
}
