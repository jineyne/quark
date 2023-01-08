#include <gtest/gtest.h>

#include <Misc/CString.h>

TEST(CStringTest, strlen) {
    EXPECT_EQ(FCStringAnsi::Strlen("Actor"), 5);
    EXPECT_NE(FCStringAnsi::Strlen("Actor"), 4);

    EXPECT_EQ(FCString::Strlen(L"Actor"), 5);
    EXPECT_NE(FCString::Strlen(L"Actor"), 4);
}

TEST(CStringTest, strcmp) {
    EXPECT_EQ(FCStringAnsi::Strcmp("Actor", "Actor"), 0);
    EXPECT_EQ(FCStringAnsi::Strcmp("Actor", "Bctor"), -1);
    EXPECT_EQ(FCStringAnsi::Strcmp("Bctor", "Actor"), 1);

    EXPECT_EQ(FCString::Strcmp(L"Actor", L"Actor"), 0);
    EXPECT_EQ(FCString::Strcmp(L"Actor", L"Bctor"), -1);
    EXPECT_EQ(FCString::Strcmp(L"Bctor", L"Actor"), 1);
}

TEST(CStringTest, strncmp) {
    EXPECT_EQ(FCStringAnsi::Strncmp("Actor", "Actor", 5), 0);
    EXPECT_EQ(FCStringAnsi::Strncmp("Actor", "Another", 1), 0);

    EXPECT_EQ(FCStringAnsi::Strncmp("Actor", "Bctor", 1), -1);
    EXPECT_EQ(FCStringAnsi::Strncmp("Bctor", "Actor", 1), 1);

    EXPECT_EQ(FCString::Strncmp(L"Actor", L"Actor", 5), 0);
    EXPECT_EQ(FCString::Strncmp(L"Actor", L"Another", 1), 0);

    EXPECT_EQ(FCString::Strncmp(L"Actor", L"Bctor", 1), -1);
    EXPECT_EQ(FCString::Strncmp(L"Bctor", L"Actor", 1), 1);
}

TEST(CStringTest, strcpy) {
    const TCHAR *src = L"Actor";
    TCHAR dst[1024];

    EXPECT_TRUE(FCString::Strcpy(dst, src, 6) != nullptr);
    EXPECT_EQ(FCString::Strncmp(dst, src, 5), 0);
    EXPECT_EQ(FCString::Strcmp(dst, src), 0);
}

TEST(CStringTest, Sprintf) {
    const TCHAR *src = L"My %ls is %d";
    TCHAR dst[1024];

    EXPECT_NE(FCString::Sprintf(dst, 1024, src, TEXT("Age"), 10), 0);
    EXPECT_EQ(FCString::Strcmp(dst, TEXT("My Age is 10")), 0);
}
