#include "Win32StackWalker.h"

static void MyStrCpy(char* szDest, size_t nMaxDestSize, const char* szSrc) {
    if (nMaxDestSize <= 0)
        return;
    strncpy_s(szDest, nMaxDestSize, szSrc, _TRUNCATE);
    // INFO: _TRUNCATE will ensure that it is null-terminated;
    // but with older compilers (<1400) it uses "strncpy" and this does not!)
    szDest[nMaxDestSize - 1] = 0;
} // MyStrCpy

Win32StackWalker::Win32StackWalker() : StackWalker(0), mBuilder(1024) {
    mBuilder.setDynamic();
}

void Win32StackWalker::showStackTrace() {
    mBuilder.reset();
    ShowCallstack();
}

void Win32StackWalker::OnOutput(LPCSTR szText) {
    // no output

    // mBuilder << ANSI_TO_TCHAR(szText);
}

void Win32StackWalker::OnCallstackEntry(StackWalker::CallstackEntryType eType, StackWalker::CallstackEntry &entry) {
    CHAR   buffer[STACKWALK_MAX_NAMELEN];
    size_t maxLen = STACKWALK_MAX_NAMELEN;
#if _MSC_VER >= 1400
    maxLen = _TRUNCATE;
#endif

    // skip crash report classes
    if (std::string(entry.name).rfind("StackWalker", 0) == 0) {
        return;
    } else if (std::string(entry.name).rfind("Win32StackWalker", 0) == 0) {
        return;
    } else if (std::string(entry.name).rfind("FCrashHandler", 0) == 0) {
        return;
    }

    if ((eType != lastEntry) && (entry.offset != 0)) {
        if (entry.name[0] == 0)
            MyStrCpy(entry.name, STACKWALK_MAX_NAMELEN, "(function-name not available)");
        if (entry.undName[0] != 0)
            MyStrCpy(entry.name, STACKWALK_MAX_NAMELEN, entry.undName);
        if (entry.undFullName[0] != 0)
            MyStrCpy(entry.name, STACKWALK_MAX_NAMELEN, entry.undFullName);
        if (entry.lineFileName[0] == 0) {
            MyStrCpy(entry.lineFileName, STACKWALK_MAX_NAMELEN, "(filename not available)");
            if (entry.moduleName[0] == 0)
                MyStrCpy(entry.moduleName, STACKWALK_MAX_NAMELEN, "(module-name not available)");
            _snprintf_s(buffer, maxLen, "%p (%s): %s: %s\n", (LPVOID)entry.offset, entry.moduleName,
                        entry.lineFileName, entry.name);
        }
        else
            _snprintf_s(buffer, maxLen, "%s (%d): %s\n", entry.lineFileName, entry.lineNumber,
                        entry.name);
        buffer[STACKWALK_MAX_NAMELEN - 1] = 0;
        mBuilder << TEXT("\tat: ") << ANSI_TO_TCHAR(buffer);
    }
}

String Win32StackWalker::getStackTrace() {
    return mBuilder.toString();
}


