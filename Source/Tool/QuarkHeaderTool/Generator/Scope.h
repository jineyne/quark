#pragma once

#include "QHTPrerequisites.h"

enum class EScopeType {
    Global,
    Namespace,
    Class,
    Struct,
    Enum,
};

struct FScope {
    FScope *parent = nullptr;

    FString fullName;
    FString currentName;
    EScopeType type;
};