#include "CommandLine.h"
#include "CString.h"
#include "Container/ContainerMacros.h"
#include "Logging/LogMacros.h"

bool CommandLine::bIsInitialized = false;
TArray<String> CommandLine::CmdParameterList = {};
TArray<String> CommandLine::CmdFlagList = {};
TMap<String, String> CommandLine::CmdArgumentList = {};

bool CommandLine::Init(size_t argc, ANSICHAR **argv) {
    if (bIsInitialized) {
        // TODO: LOG()
        return false;
    }

    // program initialize position
    CmdParameterList.add(ANSI_TO_TCHAR(argv[0]));

    for (auto i = 1; i < argc; i++) {
        auto arg = argv[i];
        auto len = CStringAnsi::Strlen(arg);
        if (len > 2 && arg[0] == '-') {
            // argument
            String sArg = ANSI_TO_TCHAR(arg);
            if (sArg.contains(TEXT("="))) {
                auto split = sArg.split(TEXT("="));
                assert(split.length() == 2);

                CmdArgumentList[split[0]] = split[1];
            } else {
                CmdFlagList.add(ANSI_TO_TCHAR(arg));
            }
        }  else {
            // parameter
            CmdParameterList.add(ANSI_TO_TCHAR(arg));
        }
    }

    bIsInitialized = true;
    return true;
}

void CommandLine::Reset() {
    CmdArgumentList.reset();
    CmdParameterList.clear();
}

const TCHAR *CommandLine::GetArgument(String name) {
    if (CmdArgumentList.contains(name)) {
        return *CmdArgumentList[name];
    }

    return nullptr;
}

const TCHAR *CommandLine::GetParameter(size_t index) {
    if (CmdParameterList.length() <= index) {
        return nullptr;
    }

    return CmdParameterList[index].getData();
}

bool CommandLine::Hasflag(String name) {
    return CmdFlagList.contains(name);
}
