#include "LogCategory.h"

static size_t gMaxCategoryName = 0;

LogCategory::LogCategory(const String &categoryName, ELogLevel level)
        : mCategoryName(categoryName), mLevel(level) {
    if (gMaxCategoryName < mCategoryName.length()) {
        gMaxCategoryName = mCategoryName.length();
    }
}

String LogCategory::toString() {
    /*if (mCached.empty() || mCachedMaxLength != gMaxCategoryName) {
        mCachedMaxLength = gMaxCategoryName;

        mCached = String(gMaxCategoryName, TEXT(' '));
        for (auto i = 0; i < mCategoryName.length(); i++) {
            mCached[gMaxCategoryName + i + 1] = mCategoryName[i];
        }
    }*/

    return mCategoryName;
}
