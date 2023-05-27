#pragma once

#include "CorePrerequisites.h"

enum class EMeshUsage {
    Static = 1 << 0,
    Dynamic = 1 << 1,
    CpuCached = 0x1000
};

DLL_EXPORT DECLARE_LOG_CATEGORY_EXTERN(LogMesh, Debug);