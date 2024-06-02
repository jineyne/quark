set (IMGUI_GUI_INC
        "Public/Gui/Gui.h")
set (IMGUI_GUI_SRC
        "Public/Gui/Gui.cpp")

set (IMGUI_RENDERER_INC
        "Private/Renderer/ImGuiRendererExtension.h")
set (IMGUI_RENDERER_SRC
        "Private/Renderer/ImGuiRendererExtension.cpp")

set (IMGUI_NOFILTER_INC
        "ImGuiPrerequisites.h")
set (IMGUI_NOFILTER_SRC
        "Plugin.cpp")

set (PLUGIN_SRCS
        ${IMGUI_GUI_INC} ${IMGUI_GUI_SRC}
        ${IMGUI_RENDERER_INC} ${IMGUI_RENDERER_SRC}
        ${IMGUI_NOFILTER_INC} ${IMGUI_NOFILTER_SRC})
