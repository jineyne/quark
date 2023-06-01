#pragma once

#include "CorePrerequisites.h"
#include "Math/Quaternion.h"
#include "SceneTypes.h"
#include "Transform.g.h"

QSTRUCT()
struct DLL_EXPORT Transform {
    GENERATED_BODY()

private:
    Actor *mOwner = nullptr;

    QPROPERTY()
    Vector3 mPosition = Vector3::ZeroVector;

    QPROPERTY()
    FQuaternion mRotation = FQuaternion(0, 0, 0, 1);

    QPROPERTY()
    Vector3 mScale = Vector3(1.f, 1.f, 1.f);

    Matrix4 mCachedLocMat = Matrix4(1.f);
    Matrix4 mCachedWorMat = Matrix4(1.f);

    Transform *mParent = nullptr;

    bool mIsDirty = true;

public:
    Transform(Actor *owner);
    ~Transform() = default;

public:
    static Transform *New(Actor *owner);

public:
    void move(const Vector3 &val);
    void rotate(const FQuaternion &val);
    void scale(const Vector3 &val);

    void lookAt(const Vector3 &location, const Vector3 &up);

    void setParent(Transform *parent);
    void setPosition(const Vector3 &pos);
    void setRotation(const FQuaternion &rot);
    void setScale(const Vector3 &scale);
    void setDirty(bool dirty = true);

    const Matrix4 &getLocalMatrix();
    const Matrix4 &getWorldMatrix();

    Vector3 getForward() const;
    Vector3 getRight() const;
    Vector3 getUp() const;

    Actor *getOwner() const { return mOwner; }

    const auto &getParent() const { return mParent; }
    const auto &getPosition() const { return mPosition; }
    const auto &getRotation() const { return mRotation; }
    const auto &getScale() const { return mScale; }

    bool isDirty() const { return mIsDirty; }

protected:
    Transform() = default;

private:
    void setOwner(Actor *actor);

    void updateMatrix();
};
