#pragma once

#include "DX11Prerequisites.h"
#include "Input/InputDevice.h"
#include "Win32/IWindowMessageHandler.h"

class DX11_EXPORT DX11Keyboard : public InputDevice, public IWindowMessageHandler  {
private:
    char16_t mHighSurrogate = 0;
    InputSymbol* mSymbol[256];

public:
    ~DX11Keyboard();

public:
    bool initialize() override;
    void update() override;

    void processKey(WPARAM wParam, LPARAM lParam);
    void processUnicode(uint32_t codepoint);

    bool handleMessage(HWND hWnd, uint32_t uMsg, WPARAM wParam, LPARAM lParam, LRESULT *outResult) override;

protected:
    void setupKeyNames();
};