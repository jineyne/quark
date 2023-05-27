#include "FTPrerequisites.h"

#include "Importer/FreeTypeImporter.h"
#include "Importer/Importer.h"

#include "Plugin/PluginFactory.h"
#include "Plugin/PluginManager.h"

class PLUGIN_EXPORT FreeTypeImporterPluginFactory : public IPluginFactory {
private:
    class InitOnStart {
    public:
        InitOnStart() {
            static IPluginFactory *factory;
            if (factory == nullptr) {
                factory = q_new<FreeTypeImporterPluginFactory>();
                PluginManager::RegisterFactory(factory);
            }
        }
    };

    static InitOnStart initOnStart;

public:
    static constexpr TCHAR *Name = TEXT("quark-freetype-importer");

public:
    const String &name() const override {
        static String inst = Name;
        return inst;
    }

    void loadPlugin() override {
        auto *importer = q_new<FreeTypeImporter>();
        gImporter().registerAssetImporter(importer);
    }

    void updatePlugin() override {
    }

    void unloadPlugin() override {
    }
};

FreeTypeImporterPluginFactory::InitOnStart FreeTypeImporterPluginFactory::initOnStart;