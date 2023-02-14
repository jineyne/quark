#pragma once

#include "Win32.h"
#include "UtilityCore.h"

DLL_EXPORT struct FWin32WindowDesc {
    WNDPROC proc;
    HINSTANCE instance;

    FString title = TEXT("");

    int32_t left = -1;
    int32_t top = -1;

    uint32_t width = 0;
    uint32_t height = 0;

    bool fullscreen = false;
    bool allowResize = true;
    bool hidden = false;
};

DLL_EXPORT class FWin32Window {
private:
    FWin32WindowDesc mDesc;
    HWND mWnd;

public:
    FWin32Window(const FWin32WindowDesc &desc);
    ~FWin32Window();

public:
    HWND getHandle() const;

    int32_t getLeft() const;
    int32_t getTop() const;
    uint32_t getWidth() const;
    uint32_t getHeight() const;
};