#pragma once

class IStackWalker {
public:
    virtual void showStackTrace() = 0;
    virtual FString getStackTrace() = 0;
};
