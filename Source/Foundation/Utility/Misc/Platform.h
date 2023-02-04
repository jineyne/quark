#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Logging/Logger.h"
#include "UUID.h"

class DLL_EXPORT FPlatform {
public:
    static void Initialize();

    static void Sleep(uint32_t time);

    [[noreturn]] static void Terminate(bool force = false);

    static Uuid GenerateUUID();
};