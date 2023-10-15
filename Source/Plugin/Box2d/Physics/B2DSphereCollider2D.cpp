#include "B2DSphereCollider2D.h"

#include "B2DPhysics.h"
#include "B2DPhysicsScene.h"

B2DSphereCollider2D::B2DSphereCollider2D(B2DPhysics *physics, B2DPhysicsScene *scene, float radius, Transform *transform)
        : mPhysics(physics), mScene(scene), mRadius(radius) {
    setOwner(transform->getOwner());

    auto position = transform->getPosition();

    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.userData.pointer = (uintptr_t) (Collider2D *) this;

    mBody = mScene->getWorld()->CreateBody(&bodyDef);
    b2CircleShape circleShape;
    circleShape.m_radius = mRadius / 2;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    mFixture = mBody->CreateFixture(&fixtureDef);
}

B2DSphereCollider2D::~B2DSphereCollider2D() {
    mBody->DestroyFixture(mFixture);
    mScene->getWorld()->DestroyBody(mBody);
}

void B2DSphereCollider2D::setRadius(float radius) {
    if (mRadius == radius) {
        return;
    }

    mRadius = radius / 2;

    auto shape = (b2CircleShape *) mFixture->GetShape();
    shape->m_radius = mRadius;
}

void B2DSphereCollider2D::setPhysicsBodyType(EPhysicsBodyType type) {
    switch (type) {
        case EPhysicsBodyType::Static:
            mBody->SetType(b2_staticBody);
            break;

        case EPhysicsBodyType::Kinematic:
            mBody->SetType(b2_kinematicBody);
            break;

        case EPhysicsBodyType::Dynamic:
            mBody->SetType(b2_dynamicBody);
            break;
    }
}

void B2DSphereCollider2D::setIsTrigger(bool isTrigger) {
    mBody->SetEnabled(isTrigger);
}

void B2DSphereCollider2D::setOffset(Vector2 offset) {

}

void B2DSphereCollider2D::updateTransform(Vector2 position, FQuaternion rotation) {
    mBody->SetTransform({position.x, position.y}, mBody->GetAngle());
}

Vector2 B2DSphereCollider2D::getPosition() const {
    auto pos = mBody->GetPosition();
    return {pos.x, pos.y};
}

FQuaternion B2DSphereCollider2D::getRotation() const {
    auto angle = mBody->GetAngle();
    return FQuaternion(Vector3::Forward, angle);
}
