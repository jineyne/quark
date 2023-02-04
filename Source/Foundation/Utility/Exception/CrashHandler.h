#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Container/String.h"
#include "IStackWalker.h"

class DLL_EXPORT FCrashHandler {
private:
    IStackWalker *mStackWalker;

public:
    void reportCrash(FString type, FString desc, FString functionName, FString fileName, size_t line);
    void setStackWalker(IStackWalker *sw);
};

DLL_EXPORT FCrashHandler &gCrashHandler();