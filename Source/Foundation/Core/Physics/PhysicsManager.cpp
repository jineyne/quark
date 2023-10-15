#include "PhysicsManager.h"

#include "Plugin/DynLibManager.h"

PhysicsManager::PhysicsManager(const String &pluginName, bool cooking) : mPlugin(nullptr), mFactory(nullptr) {
    mPlugin = DynLibManager::Instance().load(pluginName);

    if(mPlugin != nullptr) {
        typedef IPhysicsFactory* (*LoadPluginFunc)();

        LoadPluginFunc loadPluginFunc = (LoadPluginFunc)mPlugin->getSymbol("loadPlugin");
        mFactory = loadPluginFunc();

        if (mFactory != nullptr)
            mFactory->startUp(cooking);
    }
}

PhysicsManager::~PhysicsManager() {
    if (mPlugin != nullptr) {
        if (mFactory != nullptr) {
            typedef void (*UnloadPluginFunc)(IPhysicsFactory*);

            UnloadPluginFunc unloadPluginFunc = (UnloadPluginFunc)mPlugin->getSymbol("unloadPlugin");

            mFactory->shutDown();
            unloadPluginFunc(mFactory);
        }

        DynLibManager::Instance().unload(mPlugin);
    }
}
