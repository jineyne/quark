#include "LogCategory.h"

static size_t gMaxCategoryName = 0;

FLogCategory::FLogCategory(const FString &categoryName, ELogLevel level)
        : mCategoryName(categoryName), mLevel(level) {
    if (gMaxCategoryName < mCategoryName.length()) {
        gMaxCategoryName = mCategoryName.length();
    }
}

FString FLogCategory::toString() {
    /*if (mCached.empty() || mCachedMaxLength != gMaxCategoryName) {
        mCachedMaxLength = gMaxCategoryName;

        mCached = FString(gMaxCategoryName, TEXT(' '));
        for (auto i = 0; i < mCategoryName.length(); i++) {
            mCached[gMaxCategoryName + i + 1] = mCategoryName[i];
        }
    }*/

    return mCategoryName;
}
