#include "TestCore.h"

#include "Manager/SceneManager.h"
#include "Manager/SceneObjectManager.h"
#include "Scene/Component.h"
#include "Scene/Actor.h"

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Test_Core_SourceTest

class SceneTest : public ::testing::Test {
    void SetUp() override {
        SceneObjectManager::StartUp();
        SceneManager::StartUp();
    }

    void TearDown() override {
        SceneManager::ShutDown();
        SceneObjectManager::ShutDown();
    }
};

class TestComponent : public Component {
};

TEST_F(SceneTest, Actor_Destroy) {
    auto actor = Actor::New(TEXT("Actor For Test"));
    actor->destroy();
    gSceneObjectManager().destroyQueuedObject();
}

TEST_F(SceneTest, Actor_Component_AddComponent) {
    auto actor = Actor::New(TEXT("Actor For Component Test"));

    auto component = actor->addComponent<TestComponent>();
}

TEST_F(SceneTest, Actor_Component_FindComponent) {
    auto actor = Actor::New(TEXT("Actor For Component Test"));

    auto component = actor->addComponent<TestComponent>();
    EXPECT_EQ(component, actor->getComponent<TestComponent>());
}

TEST_F(SceneTest, Actor_Component_DestroyComponent) {
    auto actor = Actor::New(TEXT("Actor For Component Test"));

    auto component = actor->addComponent<TestComponent>();
    component->destroy();
}