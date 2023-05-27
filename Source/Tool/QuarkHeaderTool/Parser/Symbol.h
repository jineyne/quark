#pragma once

#include "QHTPrerequisites.h"

const static String GENERATED = TEXT("GENERATED");

struct Symbol {
public:
    String name = TEXT("");
    TArray<Symbol *> children = {};

    TMap<String, String> metas = {};
    TMap<String, String> extras = {};

    bool marked = false;

public:
    ~Symbol();
};