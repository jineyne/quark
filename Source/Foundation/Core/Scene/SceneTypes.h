#pragma once

#include "CorePrerequisites.h"

DECLARE_LOG_CATEGORY_EXTERN(FLogScene, Debug)

enum class EActorDirtyFlags {
    None = 0,
    Transform = 1 << 0,
    Mobility = 1 << 1,
    Active = 1 << 2,
    Everything = 1 << 3,
};

ENUM_CLASS_FLAGS(EActorDirtyFlags)

enum class ETransformChangedFlags {
    None = 0,
    Transform = 1 << 1,
    Parent = 1 << 2,
    Mobility = 1 << 3,
};

ENUM_CLASS_FLAGS(ETransformChangedFlags)

enum class EComponentState {
    Running,
    Paused,
    Stopped,
};