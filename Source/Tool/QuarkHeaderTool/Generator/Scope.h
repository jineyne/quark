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

    std::string fullName;
    std::string currentName;
    EScopeType type;
};