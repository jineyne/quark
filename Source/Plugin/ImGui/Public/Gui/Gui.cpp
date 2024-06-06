//
// Created by jiney on 2024-06-02.
//

#include "Gui.h"

#include <imgui.h>

bool Gui::beginWindow(const String &title, bool *isOpen) {
    return ImGui::Begin(TCHAR_TO_ANSI(*title), isOpen);
}

void Gui::endWindow() {
    ImGui::End();
}

bool Gui::beginMenuBar() {
    return ImGui::BeginMenuBar();
}

void Gui::endMenuBar() {
    return ImGui::EndMenuBar();
}

bool Gui::beginMenu(const String &title) {
    return ImGui::BeginMenu(TCHAR_TO_ANSI(*title));
}

void Gui::endMenu() {
    ImGui::EndMenu();
}

bool Gui::menuItem(const String &label, const String &shortcut) {
    return ImGui::MenuItem(TCHAR_TO_ANSI(*label), TCHAR_TO_ANSI(*shortcut));
}

bool Gui::button(const String &text) {
    return ImGui::Button(TCHAR_TO_ANSI(*text));
}

void Gui::label(String fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    auto text = String::Vprintf(*fmt, ap);
    va_end(ap);

    ImGui::Text(TCHAR_TO_ANSI(*text));
}

void Gui::inputText(const String &label, std::string &text) {
    ImGui::InputText(TCHAR_TO_ANSI(*label), text.data(), text.size());
}

void Gui::checkBox(const String &label, bool &enabled) {
    ImGui::Checkbox(TCHAR_TO_ANSI(*label), &enabled);
}

bool Gui::slider(const String &label, float &value, float min, float max) {
    return ImGui::SliderFloat(TCHAR_TO_ANSI(*label), &value, min, max);
}

bool Gui::slider(const String &label, Vector2 &value, float min, float max) {
    return ImGui::SliderFloat2(TCHAR_TO_ANSI(*label), &value.x, min, max);
}

bool Gui::slider(const String &label, Vector3 &value, float min, float max) {
    return ImGui::SliderFloat3(TCHAR_TO_ANSI(*label), &value.x, min, max);
}

bool Gui::slider(const String &label, Vector4 &value, float min, float max) {
    return ImGui::SliderFloat4(TCHAR_TO_ANSI(*label), &value.x, min, max);
}

bool Gui::slider(const String &label, FQuaternion &value, float min, float max) {
    return ImGui::SliderFloat4(TCHAR_TO_ANSI(*label), &value.x, min, max);
}

bool Gui::color(const String &label, Color &color) {
    return ImGui::ColorEdit4(TCHAR_TO_ANSI(*label), color.ref());
}

Gui &gGui() {
    static Gui inst;
    return inst;
}
