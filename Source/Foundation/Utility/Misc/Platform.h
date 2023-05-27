#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Logging/Logger.h"
#include "Uuid.h"

struct DLL_EXPORT IPlatformEventHandlerBase {};

class DLL_EXPORT Platform {
protected:
    static TArray<IPlatformEventHandlerBase *> PlatformEventHandlerList;

public:
    static void Initialize();

    static void Sleep(uint32_t time);
    [[noreturn]] static void Terminate(bool force = false);

    static Uuid GenerateUUID();

    static void RegisterPlatformEventHandler(IPlatformEventHandlerBase *handler);
    static void UnRegisterPlatformEventHandler(IPlatformEventHandlerBase *handler);
};