#pragma once

#include "DX11Prerequisites.h"
#include "Input/InputDevice.h"
#include "Win32/IWindowMessageHandler.h"

class DX11_EXPORT FDX11Mouse : public FInputDevice, public IWindowMessageHandler  {
private:
    const static uint32_t SymbolCount = static_cast<uint32_t>(EKeyCode::MouseLast) - MouseBase;
    FInputSymbol* mSymbol[SymbolCount];

    float mMouseWheel = 0;

public:
    ~FDX11Mouse();

public:
    bool initialize() override;
    void update() override;

    bool handleMessage(HWND hWnd, uint32_t uMsg, WPARAM wParam, LPARAM lParam, LRESULT *outResult) override;

    void processKey(uint32_t uMsg, WPARAM wParam, LPARAM lParam);
    void processMotion(float x, float y);
    void processScroll(float x, float y);
};
