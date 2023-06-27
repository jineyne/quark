#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Container/array.h"
#include "Container/String.h"
#include "Container/map.h"

class CommandLine {
private:
    static bool bIsInitialized;
    static TArray<String> CmdParameterList;
    static TArray<String> CmdFlagList;
    static TMap<String, String> CmdArgumentList;

public:

    static bool Init(size_t argc, ANSICHAR **argv);
    static void Reset();

    static const TCHAR *GetArgument(String name);
    static const TCHAR *GetParameter(size_t index);
    static bool Hasflag(String name);
};