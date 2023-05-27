#include "Scene.h"
#include "Manager/SceneManager.h"
#include "Manager/SceneObjectManager.h"
#include "Actor.h"

Scene::Scene() {
    mRootActor = newObject<Actor>(nullptr, Actor::StaticClass(), TEXT("RootActor"));
}

Scene *Scene::New() {
    return newObject<Scene>();
}

void Scene::shutDown() {
    mRootActor->destroy();
}

void Scene::instantiate(Actor *actor) {
    actor->attachTo(mRootActor);

    actor->setScene(this);

    gSceneManager().createNewActor(actor);
    gSceneObjectManager().registerObject(actor);
}
