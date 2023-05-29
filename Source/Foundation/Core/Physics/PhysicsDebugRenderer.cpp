#include "PhysicsDebugRenderer.h"
#include "Manager/SceneManager.h"

PhysicsDebugRenderer::PhysicsDebugRenderer()
    : RendererExtension(ERenderLocation::PreBasePass, 10) {}

void PhysicsDebugRenderer::initialize(const std::any &any) {
    RendererExtension::initialize(any);
}

void PhysicsDebugRenderer::render(CameraBase *camera) {

}

bool PhysicsDebugRenderer::checkCamera(CameraBase *camera) {
    return camera->isMain();
}

void PhysicsDebugRenderer::update(CameraBase *camera) {
    // TODO?
}

void PhysicsDebugRenderer::notifyColliderCreated(Collider *collider) {
    mRegisteredColliderList.add(collider);
}

void PhysicsDebugRenderer::notifyColliderRemoved(Collider *collider) {
    mRegisteredColliderList.remove(collider);
}
