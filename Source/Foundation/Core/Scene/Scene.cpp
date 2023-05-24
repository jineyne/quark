#include "Scene.h"
#include "Manager/SceneManager.h"
#include "Manager/SceneObjectManager.h"
#include "Actor.h"

FScene::FScene() {
    mRootActor = newObject<FActor>(nullptr, FActor::StaticClass(), TEXT("RootActor"));
}

FScene *FScene::New() {
    return newObject<FScene>();
}

void FScene::shutDown() {
    mRootActor->destroy();
}

void FScene::instantiate(FActor *actor) {
    actor->attachTo(mRootActor);

    actor->setScene(this);

    gSceneManager().createNewActor(actor);
    gSceneObjectManager().registerObject(actor);
}
