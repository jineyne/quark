#include "Win32.h"

#include <rpc.h>

#include <Misc/Platform.h>
#include <Exception/CrashHandler.h>

#include "Win32StackWalker.h"

#include "IWindowMessageHandler.h"
#include "Win32Platform.h"

DEFINE_LOG_CATEGORY(LogWin32);

void FPlatform::Initialize() {
    std::setlocale(LC_ALL, "ko_KR.UTF-8");

    gCrashHandler().setStackWalker(q_new<Win32StackWalker>());
}

void FPlatform::Sleep(uint32_t time) {
    ::Sleep(static_cast<DWORD>(time));
}

void FPlatform::Terminate(bool force) {
    if (!force) {
        PostQuitMessage(0);
    } else {
        std::terminate();
    }
}

FUuid FPlatform::GenerateUUID() {
    ::UUID uuid;
    ::UuidCreate(&uuid);

    uint32_t data1 = uuid.Data1;
    uint32_t data2 = uuid.Data2 | (uuid.Data3 << 16);
    uint32_t data3 = uuid.Data3 | (uuid.Data4[0] << 16) | (uuid.Data4[1] << 24);
    uint32_t data4 = uuid.Data4[2] | (uuid.Data4[3] << 8) | (uuid.Data4[4] << 16) | (uuid.Data4[5] << 24);

    return FUuid(data1, data2, data3, data4);
}

void FWin32Platform::WndUpdate() {
    MSG msg{};

    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT FWin32Platform::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    LRESULT result;
    bool hasValue = false;

    for (auto handler : PlatformEventHandlerList) {
        LRESULT hr = 0xDEADDEAD;
        auto win32handler = static_cast<IWindowMessageHandler *>(handler);
        if (win32handler->handleMessage(hWnd, msg, wParam, lParam, &hr)) {
            checkf(hr != 0xDEADDEAD, TEXT("Message handler indicated a resulting value, but no value was written."));
            if (hasValue) {
                checkf(result == hr, TEXT("Two window message handlers tried to return different result values"));
            } else {
                hasValue = true;
                result = hr;
            }
        }
    }

    if (hasValue) {
        return result;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
