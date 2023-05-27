#include "TestUtility.h"

#include <Container/String.h>
#include <String/StringConverter.h>
#include <Misc/CString.h>

TEST(StringConverter, StringCast) {
    String src = TEXT("Actor");

    EXPECT_EQ(CStringAnsi::Strcmp(TCHAR_TO_ANSI(*src), "Actor"), 0);
    EXPECT_EQ(CString::Strcmp(ANSI_TO_TCHAR("Actor"), TEXT("Actor")), 0);
}
