#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Container/String.h"
#include "LogMacros.h"

class DLL_EXPORT LogCategory {
private:
    String mCategoryName;
    ELogLevel mLevel;

    String mCached;
    int mCachedMaxLength;

public:
    LogCategory(const String &categoryName, ELogLevel level);

public:
    FORCEINLINE bool isSuppressed(ELogLevel level) {
        if (mLevel < level) {
            return false;
        }

        return true;
    }

    const String &getCategoryName() const { return mCategoryName; }
    String toString();
};