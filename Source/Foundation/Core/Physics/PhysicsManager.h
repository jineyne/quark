#pragma once

#include "CorePrerequisites.h"

#include "Plugin/DynLib.h"
#include "Misc/Module.h"

class DLL_EXPORT IPhysicsFactory {
public:
    virtual ~IPhysicsFactory() = default;
    virtual void startUp(bool cooking) = 0;
    virtual void shutDown() = 0;
};

class DLL_EXPORT PhysicsManager : public TModule <PhysicsManager> {
private:
    DynLib* mPlugin;
    IPhysicsFactory* mFactory;

public:
    PhysicsManager(const String& pluginName, bool cooking);
    ~PhysicsManager();
};