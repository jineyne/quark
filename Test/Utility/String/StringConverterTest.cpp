#include "TestUtility.h"

#include <Container/String.h>
#include <String/StringConverter.h>
#include <Misc/CString.h>

TEST(StringConverter, StringCast) {
    FString src = TEXT("Actor");

    EXPECT_EQ(FCStringAnsi::Strcmp(TCHAR_TO_ANSI(*src), "Actor"), 0);
    EXPECT_EQ(FCString::Strcmp(ANSI_TO_TCHAR("Actor"), TEXT("Actor")), 0);
}
