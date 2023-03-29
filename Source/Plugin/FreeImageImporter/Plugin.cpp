#include "FIPrerequisites.h"
#include "Plugin/PluginFactory.h"
#include "Plugin/PluginManager.h"
#include "Importer/FreeImgImporter.h"
#include "Importer/Importer.h"

DEFINE_LOG_CATEGORY(FLogFreeImgImporter)

class PLUGIN_EXPORT FFreeImgImporterPluginFactory : public IPluginFactory {
private:
    class FInitOnStart {
    public:
        FInitOnStart() {
            static IPluginFactory *factory;
            if (factory == nullptr) {
                factory = q_new<FFreeImgImporterPluginFactory>();
                FPluginManager::RegisterFactory(factory);
            }
        }
    };

    static FInitOnStart initOnStart;

public:
    static constexpr TCHAR *Name = TEXT("quark-freeimg-importer");

public:
    const FString &name() const override {
        static FString inst = Name;
        return inst;
    }

    void loadPlugin() override {
        auto *importer = q_new<FFreeImgImporter>();
        gImporter().registerAssetImporter(importer);
    }

    void updatePlugin() override {
    }

    void unloadPlugin() override {
    }
};

FFreeImgImporterPluginFactory::FInitOnStart FFreeImgImporterPluginFactory::initOnStart;