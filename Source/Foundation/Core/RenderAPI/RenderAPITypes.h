#pragma once

#include "CorePrerequisites.h"

enum class EGpuQueueType {
    Graphics
};

enum class EBlendFactor {
    One,
    Zero,
    DestColor,
    SourceColor,
    InvDestColor,
    InvSourceColor,
    DestAlpha,
    SourceAlpha,
    InvDestAlpha,
    InvSourceAlpha,
};

enum class ECompareFunction {
    AlwaysFail,
    AlwaysPass,
    Less,
    LessEqual,
    Equal,
    NotEqual,
    GreaterEqual,
    Greater
};

enum class EStencilOperation {
    Keep,
    Zero,
    Replace,
    Increment,
    Decrement,
    IncrementWrap,
    DecrementWrap,
    Invert,
};