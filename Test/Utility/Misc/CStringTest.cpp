#include <gtest/gtest.h>

#include <Misc/CString.h>

TEST(CStringTest, strlen) {
    EXPECT_EQ(CStringAnsi::Strlen("Actor"), 5);
    EXPECT_NE(CStringAnsi::Strlen("Actor"), 4);

    EXPECT_EQ(CString::Strlen(L"Actor"), 5);
    EXPECT_NE(CString::Strlen(L"Actor"), 4);
}

TEST(CStringTest, strcmp) {
    EXPECT_EQ(CStringAnsi::Strcmp("Actor", "Actor"), 0);
    EXPECT_EQ(CStringAnsi::Strcmp("Actor", "Bctor"), -1);
    EXPECT_EQ(CStringAnsi::Strcmp("Bctor", "Actor"), 1);

    EXPECT_EQ(CString::Strcmp(L"Actor", L"Actor"), 0);
    EXPECT_EQ(CString::Strcmp(L"Actor", L"Bctor"), -1);
    EXPECT_EQ(CString::Strcmp(L"Bctor", L"Actor"), 1);
}

TEST(CStringTest, strncmp) {
    EXPECT_EQ(CStringAnsi::Strncmp("Actor", "Actor", 5), 0);
    EXPECT_EQ(CStringAnsi::Strncmp("Actor", "Another", 1), 0);

    EXPECT_EQ(CStringAnsi::Strncmp("Actor", "Bctor", 1), -1);
    EXPECT_EQ(CStringAnsi::Strncmp("Bctor", "Actor", 1), 1);

    EXPECT_EQ(CString::Strncmp(L"Actor", L"Actor", 5), 0);
    EXPECT_EQ(CString::Strncmp(L"Actor", L"Another", 1), 0);

    EXPECT_EQ(CString::Strncmp(L"Actor", L"Bctor", 1), -1);
    EXPECT_EQ(CString::Strncmp(L"Bctor", L"Actor", 1), 1);
}

TEST(CStringTest, strcpy) {
    const TCHAR *src = L"Actor";
    TCHAR dst[1024];

    EXPECT_TRUE(CString::Strcpy(dst, src, 6) != nullptr);
    EXPECT_EQ(CString::Strncmp(dst, src, 5), 0);
    EXPECT_EQ(CString::Strcmp(dst, src), 0);
}

TEST(CStringTest, Sprintf) {
    const TCHAR *src = L"My %ls is %d";
    TCHAR dst[1024];

    EXPECT_NE(CString::Sprintf(dst, 1024, src, TEXT("Age"), 10), 0);
    EXPECT_EQ(CString::Strcmp(dst, TEXT("My Age is 10")), 0);
}
