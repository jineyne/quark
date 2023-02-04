#include "CrashHandler.h"
#include "Logging/LogDefines.h"
#include "Misc/StringBuilder.h"

DECLARE_LOG_CATEGORY_EXTERN(LogException, Fatal)

DEFINE_LOG_CATEGORY(LogException);

void FCrashHandler::reportCrash(FString type, FString desc, FString functionName, FString fileName, size_t line) {
    FStringBuilder ss(1024);
    ss.setDynamic();

    ss << TEXT("Unhandled framework exception (") << *type << TEXT(") occurred: ") << *desc << TEXT("\n")
       << TEXT("\tin :") << *functionName << TEXT("\n");

    if (line > 0) {
        ss << TEXT("\tat: ") << *fileName << TEXT(":") << line << TEXT("\n");
    }

    mStackWalker->showStackTrace();
    ss << *(mStackWalker->getStackTrace());

    LOG(LogException, Fatal, *ss.toString());
}

void FCrashHandler::setStackWalker(IStackWalker *sw) {
    mStackWalker = sw;
}

FCrashHandler &gCrashHandler() {
    static FCrashHandler instance;
    return instance;
}
