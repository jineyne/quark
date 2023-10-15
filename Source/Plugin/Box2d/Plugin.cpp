#include "B2DPrerequisites.h"

#include <Physics/PhysicsManager.h>
#include <Plugin/PluginManager.h>

#include "Physics/B2DPhysics.h"

DEFINE_LOG_CATEGORY(LogB2D)

class PLUGIN_EXPORT B2DPluginFactory : public IPhysicsFactory {
public:
    static constexpr TCHAR *Name = TEXT("quark-box2d");

public:
    void startUp(bool cooking) override {
        PhysicsDesc desc;

        Physics::StartUp<B2DPhysics>(desc);
    }

    void shutDown() override {
        Physics::ShutDown();

    }
};

extern "C" B2D_EXPORT B2DPluginFactory* loadPlugin() {
    return q_new<B2DPluginFactory>();
}

extern "C" B2D_EXPORT void unloadPlugin(B2DPluginFactory* instance) {
    q_delete(instance);
}