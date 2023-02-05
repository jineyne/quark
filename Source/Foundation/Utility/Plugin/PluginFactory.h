#pragma once

#include "UtilityCore.h"

class DLL_EXPORT IPluginFactory {
public:
    virtual ~IPluginFactory() = default;

public:
    virtual const FString &name() const = 0;

    virtual void loadPlugin() = 0;
    virtual void updatePlugin() = 0;
    virtual void unloadPlugin() = 0;
};
