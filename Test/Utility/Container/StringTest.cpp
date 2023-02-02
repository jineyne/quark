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

TEST(StringTest, startWith) {
    FString string = "Actor";

    EXPECT_TRUE(string.startWith(TEXT("A")));
    EXPECT_TRUE(string.startWith(TEXT("Ac")));
    EXPECT_TRUE(string.startWith(TEXT("Act")));
    EXPECT_TRUE(string.startWith(TEXT("Acto")));

    EXPECT_FALSE(string.startWith(TEXT("ctor")));
    EXPECT_FALSE(string.startWith(TEXT("tor")));
    EXPECT_FALSE(string.startWith(TEXT("ctor")));
    EXPECT_FALSE(string.startWith(TEXT("or")));
    EXPECT_FALSE(string.startWith(TEXT("r")));
}

TEST(StringTest, endWith) {
    FString string = "Actor";

    EXPECT_TRUE(string.endWith(TEXT("ctor")));
    EXPECT_TRUE(string.endWith(TEXT("tor")));
    EXPECT_TRUE(string.endWith(TEXT("ctor")));
    EXPECT_TRUE(string.endWith(TEXT("or")));
    EXPECT_TRUE(string.endWith(TEXT("r")));

    EXPECT_FALSE(string.endWith(TEXT("A")));
    EXPECT_FALSE(string.endWith(TEXT("Ac")));
    EXPECT_FALSE(string.endWith(TEXT("Act")));
    EXPECT_FALSE(string.endWith(TEXT("Acto")));
}
