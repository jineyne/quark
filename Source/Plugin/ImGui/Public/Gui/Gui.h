#pragma once

#include "ImGuiPrerequisites.h"

class PLUGIN_EXPORT Gui {
public:
    bool beginWindow(const String &title, bool *isOpen = nullptr);
    void endWindow();

    bool button(const String &text);

    void label(String fmt, ...);
};

DLL_EXPORT Gui&gGui();