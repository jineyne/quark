#include <gtest/gtest.h>

#include "Container/String.h"
#include "Misc/CString.h"

TEST(StringTest, init) {
    FString original = "first";

    EXPECT_EQ(original.length(), 5);
    EXPECT_EQ(TCString<WIDECHAR>::Strncmp(original.getData(), TEXT("first"), original.length()), 0);

    FString copied = original;
    EXPECT_EQ(copied.length(), 5);
    EXPECT_EQ(TCString<WIDECHAR>::Strncmp(copied.getData(), TEXT("first"), copied.length()), 0);
}

TEST(StringTest, append) {
    FString string = "Actor";

    EXPECT_NO_THROW(string.append("_fir"));
    EXPECT_NO_THROW(string.append(L"st"));
    EXPECT_TRUE(string.equals(L"Actor_first"));
}
