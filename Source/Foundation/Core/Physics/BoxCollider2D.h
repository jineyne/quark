#pragma once

#include "CorePrerequisites.h"
#include "Math/Size.h"
#include "Collider2D.h"

class DLL_EXPORT BoxCollider2D : public Collider2D {
public:
    virtual void setSize(Size size) = 0;
};

