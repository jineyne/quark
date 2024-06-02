#include "ImGuiPrerequisites.h"

#include <Renderer/RendererExtension.h>
#include <Plugin/PluginFactory.h>
#include <Plugin/PluginManager.h>

#include "Renderer/ImGuiRendererExtension.h"

class PLUGIN_EXPORT ImGuiPluginFactory : public IPluginFactory {
private:
    class InitOnStart {
    public:
        InitOnStart() {
            static IPluginFactory *factory;
            if (factory == nullptr) {
                factory = q_new<ImGuiPluginFactory>();
                PluginManager::RegisterFactory(factory);
            }
        }
    };

    static InitOnStart initOnStart;

public:
    static constexpr TCHAR *Name = TEXT("quark-imgui");

private:
    TSharedPtr<ImGuiRendererExtension> extensions;

public:
    const String &name() const override {
        static String inst = Name;
        return inst;
    }

    void loadPlugin() override {
        std::any any{};
        extensions = RendererExtension::New<ImGuiRendererExtension>(any);
    }

    void updatePlugin() override {
    }

    void unloadPlugin() override {
        extensions.reset();
        extensions = nullptr;
    }
};

ImGuiPluginFactory::InitOnStart ImGuiPluginFactory::initOnStart;