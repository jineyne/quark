#pragma once

#include "DX11Prerequisites.h"
#include "Input/InputDevice.h"
#include "Win32/IWindowMessageHandler.h"

class DX11_EXPORT FDX11Keyboard : public FInputDevice, public IWindowMessageHandler  {
private:
    char16_t mHighSurrogate = 0;
    FInputSymbol* mSymbol[256];

public:
    ~FDX11Keyboard();

public:
    bool initialize() override;
    void update() override;

    void processKey(WPARAM wParam, LPARAM lParam);
    void processUnicode(uint32_t codepoint);

    bool handleMessage(HWND hWnd, uint32_t uMsg, WPARAM wParam, LPARAM lParam, LRESULT *outResult) override;

protected:
    void setupKeyNames();
};