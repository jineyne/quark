#include "Win32.h"

#include <rpc.h>

#include <Misc/Platform.h>
#include <Exception/CrashHandler.h>

#include "Win32StackWalker.h"

DEFINE_LOG_CATEGORY(LogWin32);


void FPlatform::Initialize() {
    std::setlocale(LC_ALL, "ko_KR.UTF-8");

    gCrashHandler().setStackWalker(new Win32StackWalker());
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

Uuid FPlatform::GenerateUUID() {
    ::UUID uuid;
    ::UuidCreate(&uuid);

    uint32_t data1 = uuid.Data1;
    uint32_t data2 = uuid.Data2 | (uuid.Data3 << 16);
    uint32_t data3 = uuid.Data3 | (uuid.Data4[0] << 16) | (uuid.Data4[1] << 24);
    uint32_t data4 = uuid.Data4[2] | (uuid.Data4[3] << 8) | (uuid.Data4[4] << 16) | (uuid.Data4[5] << 24);

    return Uuid(data1, data2, data3, data4);
}