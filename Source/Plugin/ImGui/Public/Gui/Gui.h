#pragma once

#include "ImGuiPrerequisites.h"

#include <Image/Color.h>

class PLUGIN_EXPORT Gui {
public:
    bool beginWindow(const String &title, bool *isOpen = nullptr);
    void endWindow();

    bool beginMenuBar();
    void endMenuBar();

    bool beginMenu(const String &title);
    void endMenu();

    bool menuItem(const String &label, const String &shortcut);

    bool button(const String &text);

    void label(String fmt, ...);

    void inputText(const String &label, std::string &text);

    void checkBox(const String &label, bool &enabled);

    bool slider(const String &label, float &value, float min, float max);
    bool slider(const String &label, Vector2 &value, float min, float max);
    bool slider(const String &label, Vector3 &value, float min, float max);
    bool slider(const String &label, Vector4 &value, float min, float max);
    bool slider(const String &label, FQuaternion &value, float min, float max);

    bool color(const String &label, Color &color);
};

DLL_EXPORT Gui&gGui();