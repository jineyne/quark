#include "Plugin/PluginFactory.h"

#include "MyDynLibTestModule.h"
#include "Plugin/PluginManager.h"

class PLUGIN_EXPORT TestDynLibPluginFactory : public IPluginFactory {
private:
    class InitOnStart {
    public:
        InitOnStart() {
            static IPluginFactory *newFactory;
            if (newFactory == nullptr) {
                newFactory = q_new<TestDynLibPluginFactory>();
                PluginManager::RegisterFactory(newFactory);
            }
        }
    };

    static InitOnStart initOnStart;

public:
    static constexpr TCHAR *Name = TEXT("quark-test-dynlib");

public:
    const String &name() const override {
        static String inst = Name;
        return inst;
    }

    void loadPlugin() override {
        FDynLibTestModule::StartUp<FMyDynLibTestModule>();
    }

    void updatePlugin() override {

    }

    void unloadPlugin() override {
        FDynLibTestModule::ShutDown();
    }
};

TestDynLibPluginFactory::InitOnStart TestDynLibPluginFactory::initOnStart;