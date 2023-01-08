#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Container/String.h"
#include "LogMacros.h"

class DLL_EXPORT FLogCategory {
private:
    FString mCategoryName;
    ELogLevel mLevel;

    FString mCached;
    int mCachedMaxLength;

public:
    FLogCategory(const FString &categoryName, ELogLevel level);

public:
    FORCEINLINE bool isSuppressed(ELogLevel level) {
        if (mLevel < level) {
            return false;
        }

        return true;
    }

    const FString &getCategoryName() const { return mCategoryName; }
    FString toString();
};