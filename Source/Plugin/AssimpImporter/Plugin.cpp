#include "AIPrerequisites.h"
#include "Plugin/PluginFactory.h"
#include "Plugin/PluginManager.h"
#include "Importer/AssimpImporter.h"
#include "Importer/Importer.h"

class PLUGIN_EXPORT FAssimpImporterPluginFactory : public IPluginFactory {
private:
    class FInitOnStart {
    public:
        FInitOnStart() {
            static IPluginFactory *factory;
            if (factory == nullptr) {
                factory = q_new<FAssimpImporterPluginFactory>();
                FPluginManager::RegisterFactory(factory);
            }
        }
    };

    static FInitOnStart initOnStart;

public:
    static constexpr TCHAR *Name = TEXT("quark-assimp-importer");

public:
    const FString &name() const override {
        static FString inst = Name;
        return inst;
    }

    void loadPlugin() override {
        auto *importer = q_new<FAssimpImporter>();
        gImporter().registerAssetImporter(importer);
    }

    void updatePlugin() override {
    }

    void unloadPlugin() override {
    }
};

FAssimpImporterPluginFactory::FInitOnStart FAssimpImporterPluginFactory::initOnStart;