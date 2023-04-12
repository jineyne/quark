#pragma once

#include "CorePrerequisites.h"

enum class EActorDirtyFlags {
    None = 0,
    Transform = 1 << 0,
    Mobility = 1 << 1,
    Active = 1 << 2,
    Everything = 1 << 3,
};

ENUM_CLASS_FLAGS(EActorDirtyFlags)