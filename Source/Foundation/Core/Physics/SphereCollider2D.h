#pragma once

#include "CorePrerequisites.h"
#include "Collider2D.h"

class DLL_EXPORT SphereCollider2D : public Collider2D {
public:
    virtual void setRadius(float radius) = 0;
};