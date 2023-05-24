#pragma once

#include "CorePrerequisites.h"
#include "SceneObject.h"
#include "Transform.h"
#include "Actor.g.h"

QCLASS()
class DLL_EXPORT FActor : public FSceneObject {
    GENERATED_BODY();

private:
    FTransform *mTransform = nullptr;
    TArray<FComponent *> mAttachedComponent;
    FScene *mTargetScene  = nullptr;
    TArray<FActor *> mAttachedActorList;
    FActor *mParentActor = nullptr;

public:
    FActor();
    virtual ~FActor() = default;

public:
    static FActor *New(const FString &actorName);

public:
    virtual void destroy(bool immediate = false);

    void attachTo(FActor *actor);
    void detachFrom(FActor *actor);

    void move(const FVector3 &value);

    template <typename T, typename ...Args>
    T *addComponent(Args &&...args) {
        static_assert(std::is_base_of<FComponent, T>::value,
                      "FActor::addComponent type is not derived from component!");

        auto component = q_new<T>();
        component->attachTo(this);

        return component;
    }

    template <typename T>
    T *getComponent() {
        static_assert(std::is_base_of<FComponent, T>::value,
                      "FActor::getComponent type is not derived from component!");

        for (auto &component : mAttachedComponent) {
            // TODO: Use Reflection
            auto c = dynamic_cast<T *>(component);
            if (c != nullptr) {
                return c;
            }
        }

        return nullptr;
    }

    void removeComponent(FComponent *component);
    void destroyComponent(FComponent *component, bool immediate);

    template <typename T, typename ...Args>
    T *spawnActor(Args &&...args) {
        return getScene()->spawnActor<T>(std::forward<Args>(args)...);
    }

    void setActive(bool active) override;

    void setScene(FScene *targetScene);

    const auto &getTransform() const { return mTransform; }
    const auto &getScene() const { return mTargetScene; }
    const auto &getParent() const { return mParentActor; }
    const auto &getChildren() const { return mAttachedActorList; }
    const auto &getComponents() const { return mAttachedComponent; }

    void notifyTransformChanged(ETransformChangedFlags flags) const override;

protected:
    void addAndInitializeComponent(FComponent *component);
    void destroyInternal(bool immediate) override;

private:
    friend class FScene;
    friend class FSceneManager;
    friend class FComponent;
};
