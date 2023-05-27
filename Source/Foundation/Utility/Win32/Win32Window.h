#pragma once

#include "Win32.h"
#include "UtilityCore.h"

struct DLL_EXPORT Win32WindowDesc {
    WNDPROC proc;
    HINSTANCE instance;

    String title = TEXT("");

    int32_t left = -1;
    int32_t top = -1;

    uint32_t width = 0;
    uint32_t height = 0;

    bool fullscreen = false;
    bool allowResize = true;
    bool hidden = false;
};

class DLL_EXPORT Win32Window {
private:
    Win32WindowDesc mDesc;
    HWND mWnd;

public:
    Win32Window(const Win32WindowDesc &desc);
    ~Win32Window();

public:
    void show();
    void hide();

    void minimize();
    void maximize(bool maximized = true);

    void resize(int32_t width, int32_t height) ;
    void move(int32_t left, int32_t top) ;
    void setTitle(const String &title);

    HWND getHandle() const;

    int32_t getLeft() const;
    int32_t getTop() const;
    uint32_t getWidth() const;
    uint32_t getHeight() const;
};