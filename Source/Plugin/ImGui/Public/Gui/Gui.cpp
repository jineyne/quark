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

Gui &gGui() {
    static Gui inst;
    return inst;
}
