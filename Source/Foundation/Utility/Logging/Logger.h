#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Misc/SingletonMacros.h"

#include "LogCategory.h"
#include "LogMacros.h"

class DLL_EXPORT FLogger {
public:
    FLogger();

public:
    void log(const FString &categoryName, ELogLevel level, const TCHAR *message);

    DECLARE_SINGLETON(FLogger);
};
