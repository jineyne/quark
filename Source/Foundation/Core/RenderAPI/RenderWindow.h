#pragma once

#include "CorePrerequisites.h"
#include "RenderTarget.h"
#include "VideoModeInfo.h"

struct FRenderWindowDesc {
    FVideoMode videoMode;

    bool fullscreen = false;
    bool vsync = false;
    uint32_t vsyncInterval = 1;

    bool hidden = false;
    bool showTitleBar = true;
    bool allowResize = true;
    bool alwaysOnTop = false;
    FString title = TEXT("Quark");
};

DECLARE_EVENT(FWindowCloseRequested);
DECLARE_EVENT(FWindowHovered);
DECLARE_EVENT(FWindowFocused);

class DLL_EXPORT FRenderWindow : public FRenderTarget {
public:
    enum class EWindowSizeState {
        Normal,
        Maximize,
        Minimize,
        FullScreen,
    };

    FWindowCloseRequested onCloseRequested;
    FWindowHovered onHovered;
    FWindowFocused onFocused;

protected:
    int32_t mLeft = 0, mTop = 0;
    bool bHidden = false;
    EWindowSizeState mWindowSizeState = EWindowSizeState::Normal;

private:
    FRenderWindowDesc mDesc;
    uint32_t mWindowID;

public:
    FRenderWindow(const FRenderWindowDesc &desc, uint32_t windowId, FRenderWindow *parent = nullptr);
    ~FRenderWindow();

public:
    static FRenderWindow *New(const FRenderWindowDesc &desc, FRenderWindow *parent = nullptr);

public:
    virtual void show();
    virtual void hide();

    virtual void minimize();
    virtual void maximize(bool maximized = true);

    virtual void resize(int32_t width, int32_t height) = 0;
    virtual void move(int32_t left, int32_t top) = 0;
    virtual void setTitle(const FString &title) = 0;

    auto getWindowID() const { return mWindowID; }
    const auto &getLeft() const { return mLeft; }
    const auto &getTop() const { return mTop; }

    bool isHidden() const { return bHidden; }
    bool isMinimized() const { return mWindowSizeState == EWindowSizeState::Minimize; }
    bool isMaximized() const { return mWindowSizeState == EWindowSizeState::Maximize; }
    bool isFullScreen() const { return mWindowSizeState == EWindowSizeState::FullScreen; }
};