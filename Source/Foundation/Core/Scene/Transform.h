#pragma once

#include "CorePrerequisites.h"
#include "Math/Quaternion.h"
#include "SceneTypes.h"
#include "Transform.g.h"

QSTRUCT()
struct DLL_EXPORT FTransform {
    GENERATED_BODY()

private:
    FActor *mOwner = nullptr;

    QPROPERTY()
    FVector3 mPosition = FVector3::ZeroVector;

    QPROPERTY()
    FQuaternion mRotation = FQuaternion(0, 0, 0, 1);

    QPROPERTY()
    FVector3 mScale = FVector3(1.f, 1.f, 1.f);

    FMatrix4 mCachedLocMat = FMatrix4(1.f);
    FMatrix4 mCachedWorMat = FMatrix4(1.f);

    FTransform *mParent = nullptr;

    bool mIsDirty = true;

public:
    FTransform(FActor *owner);
    ~FTransform() = default;

public:
    static FTransform *New(FActor *owner);

public:
    void move(const FVector3 &val);
    void rotate(const FQuaternion &val);
    void scale(const FVector3 &val);

    void lookAt(const FVector3 &location, const FVector3 &up);

    void setParent(FTransform *parent);
    void setPosition(const FVector3 &pos);
    void setRotation(const FQuaternion &rot);
    void setScale(const FVector3 &scale);
    void setDirty(bool dirty = true);

    const FMatrix4 &getLocalMatrix();
    const FMatrix4 &getWorldMatrix();

    FVector3 getForward() const;
    FVector3 getRight() const;
    FVector3 getUp() const;

    const auto &getParent() const { return mParent; }
    const auto &getPosition() const { return mPosition; }
    const auto &getRotation() const { return mRotation; }
    const auto &getScale() const { return mScale; }

    bool isDirty() const { return mIsDirty; }

protected:
    FTransform() = default;

private:
    void setOwner(FActor *actor);

    void updateMatrix();
};
