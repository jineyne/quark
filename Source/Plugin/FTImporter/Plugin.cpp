#include "FTPrerequisites.h"

#include "Importer/FTImporter.h"
#include "Importer/Importer.h"

#include "Plugin/PluginFactory.h"
#include "Plugin/PluginManager.h"

class PLUGIN_EXPORT FTImporterPluginFactory : public IPluginFactory {
private:
    class InitOnStart {
    public:
        InitOnStart() {
            static IPluginFactory *factory;
            if (factory == nullptr) {
                factory = q_new<FTImporterPluginFactory>();
                FPluginManager::RegisterFactory(factory);
            }
        }
    };

    static InitOnStart initOnStart;

public:
    static constexpr TCHAR *Name = TEXT("quark-freetype-importer");

public:
    const FString &name() const override {
        static FString inst = Name;
        return inst;
    }

    void loadPlugin() override {
        auto *importer = q_new<FFTImporter>();
        gImporter().registerAssetImporter(importer);
    }

    void updatePlugin() override {
    }

    void unloadPlugin() override {
    }
};

FTImporterPluginFactory::InitOnStart FTImporterPluginFactory::initOnStart;