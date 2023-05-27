#include "AIPrerequisites.h"
#include "Plugin/PluginFactory.h"
#include "Plugin/PluginManager.h"
#include "Importer/AssimpImporter.h"
#include "Importer/Importer.h"

class PLUGIN_EXPORT AssimpImporterPluginFactory : public IPluginFactory {
private:
    class InitOnStart {
    public:
        InitOnStart() {
            static IPluginFactory *factory;
            if (factory == nullptr) {
                factory = q_new<AssimpImporterPluginFactory>();
                PluginManager::RegisterFactory(factory);
            }
        }
    };

    static InitOnStart initOnStart;

public:
    static constexpr TCHAR *Name = TEXT("quark-assimp-importer");

public:
    const String &name() const override {
        static String inst = Name;
        return inst;
    }

    void loadPlugin() override {
        auto *importer = q_new<AssimpImporter>();
        gImporter().registerAssetImporter(importer);
    }

    void updatePlugin() override {
    }

    void unloadPlugin() override {
    }
};

AssimpImporterPluginFactory::InitOnStart AssimpImporterPluginFactory::initOnStart;