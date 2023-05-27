#pragma once

#include "Win32.h"
#include "UtilityCore.h"

#include "Exception/IStackWalker.h"
#include "Misc/StringBuilder.h"

#include "StackWalker.h"

class Win32StackWalker : public StackWalker, public IStackWalker {
public:
    FStringBuilder mBuilder;

public:
    Win32StackWalker();

protected:
    void OnOutput(LPCSTR szText) override;
    void OnCallstackEntry(CallstackEntryType eType, CallstackEntry &entry) override;

public:
    void showStackTrace() override;
    String getStackTrace() override;
};