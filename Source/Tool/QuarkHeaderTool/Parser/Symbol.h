#pragma once

#include "QHTPrerequisites.h"

const static FString GENERATED = TEXT("GENERATED");

class FSymbol {
public:
    FString name = TEXT("");
    TArray<FSymbol *> children = {};

    TMap<FString, FString> metas = {};
    TMap<FString, FString> extras = {};

public:
    ~FSymbol();
};