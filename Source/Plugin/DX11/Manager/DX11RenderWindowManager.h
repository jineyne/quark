#pragma once

#include "DX11Prerequisites.h"
#include "Manager/RenderWindowManager.h"
#include "Win32/IWindowMessageHandler.h"
#include "DX11RenderWindowManager.g.h"

QCLASS()
class DX11_EXPORT DX11RenderWindowManager : public RenderWindowManager, public IWindowMessageHandler {
    GENERATED_BODY()

public:
    void update() override;

    bool handleMessage(HWND hWnd, uint32_t uMsg, WPARAM wParam, LPARAM lParam, LRESULT *outResult) override;

protected:
    void onStartUp() override;

    void onShutDown() override;

    RenderWindow *createInternal(const RenderWindowDesc &desc, uint32_t windowId, RenderWindow *parent) override;
};

