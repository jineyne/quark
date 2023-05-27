#pragma once

class IStackWalker {
public:
    virtual void showStackTrace() = 0;
    virtual String getStackTrace() = 0;
};
