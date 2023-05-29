#pragma once

#include "CorePrerequisites.h"
#include "SceneObject.h"
#include "Transform.h"
#include "Actor.g.h"

QCLASS()
class DLL_EXPORT Actor : public SceneObject {
    GENERATED_BODY();

private:
    Transform *mTransform = nullptr;
    TArray<Component *> mAttachedComponent;
    Scene *mTargetScene  = nullptr;
    TArray<Actor *> mAttachedActorList;
    Actor *mParentActor = nullptr;

public:
    Actor();
    virtual ~Actor() = default;

public:
    static Actor *New(const String &actorName);
    static Actor *Find(const String &actorName);

public:
    virtual void destroy(bool immediate = false);

    void attachTo(Actor *actor);
    void detachFrom(Actor *actor);

    void move(const Vector3 &value);

    template <typename T, typename ...Args>
    T *addComponent(Args &&...args) {
        static_assert(std::is_base_of<Component, T>::value,
                      "Actor::addComponent type is not derived from component!");

        auto component = newObject<T>(this);
        component->attachTo(this);

        return component;
    }

    template <typename T>
    T *getComponent() {
        static_assert(std::is_base_of<Component, T>::value,
                      "Actor::getComponent type is not derived from component!");

        for (auto &component : mAttachedComponent) {
            // TODO: Use Reflection
            auto c = dynamic_cast<T *>(component);
            if (c != nullptr) {
                return c;
            }
        }

        return nullptr;
    }

    void removeComponent(Component *component);
    void destroyComponent(Component *component, bool immediate);

    void setActive(bool active) override;

    void setScene(Scene *targetScene);

    const auto &getTransform() const { return mTransform; }
    const auto &getScene() const { return mTargetScene; }
    const auto &getParent() const { return mParentActor; }
    const auto &getChildren() const { return mAttachedActorList; }
    const auto &getComponents() const { return mAttachedComponent; }

    void notifyTransformChanged(ETransformChangedFlags flags) const override;

protected:
    void addAndInitializeComponent(Component *component);
    void destroyInternal(bool immediate) override;

private:
    friend class Scene;
    friend class SceneManager;
    friend class Component;
};
