
#pragma once

#include "ImGuiPrerequisites.h"

#include <Renderer/RendererExtension.h>

class PLUGIN_EXPORT ImGuiRendererExtension : public RendererExtension {
public:
    ImGuiRendererExtension();

public:
    void initialize(const std::any &data) override;
    void destroy() override;

    virtual void render(CameraBase *camera);
    virtual bool checkCamera(CameraBase *camera);
};
