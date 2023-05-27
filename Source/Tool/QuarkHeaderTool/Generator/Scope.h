#pragma once

#include "QHTPrerequisites.h"

enum class EScopeType {
    Global,
    Namespace,
    Class,
    Struct,
    Enum,
};

struct Scope {
    Scope *parent = nullptr;

    String fullName;
    String currentName;
    EScopeType type;
};