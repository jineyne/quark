#pragma once

#include "CorePrerequisites.h"

#include "Scene/Transform.h"

#include "BoxCollider2D.h"
#include "SphereCollider2D.h"


class DLL_EXPORT PhysicsScene {
public:
    virtual BoxCollider2D *createBoxCollider2D(const Size &size, Transform *transform) = 0;
    virtual SphereCollider2D *createSphereCollider2D(float radius, Transform *transform) = 0;
};
