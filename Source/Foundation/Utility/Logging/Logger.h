#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Misc/SingletonMacros.h"

#include "LogCategory.h"
#include "LogMacros.h"

class DLL_EXPORT Logger {
public:
    Logger();

public:
    void log(const String &categoryName, ELogLevel level, const TCHAR *message);

    DECLARE_SINGLETON(Logger);
};
