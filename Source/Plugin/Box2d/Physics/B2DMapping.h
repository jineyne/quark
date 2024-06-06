#pragma once

#include "B2DPrerequisites.h"

#include <Physics/Collider2D.h>

class B2D_EXPORT B2DMapping {
public:
    static b2BodyType GetBodyType(EPhysicsBodyType type);
};