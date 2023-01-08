#include "TestUtility.h"

#include "Misc/StringBuilder.h"

TEST(StringBuilderTest, FStringBuilder) {
    FStringBuilder sb(512);

    sb << TEXT("Hello, World");
    sb.appendChar('!');

    ASSERT_EQ(TCString<TCHAR>::Strncmp(sb.getData(), TEXT("Hello, World!"), sb.length()), 0);
    ASSERT_TRUE(sb.toString().equals(TEXT("Hello, World!")));
}

TEST(StringBuilderTest, TStringBuilder) {
    TStringBuilder<char> sb(512);

    sb.append("Hello, World", 12);
    sb.appendChar('!');

    ASSERT_EQ(TCString<char>::Strncmp(sb.getData(), "Hello, World!", sb.length()), 0);
    ASSERT_TRUE(sb.toString().equals(TEXT("Hello, World!")));
}