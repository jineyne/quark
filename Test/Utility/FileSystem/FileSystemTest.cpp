#include <gtest/gtest.h>
#include <FileSystem/FileSystem.h>

static TCHAR data[512] = TEXT("Hello, World!\nThis is debug message!");

TEST(FFileSystemTest, WriteFile) {
    auto path = FileSystem::GetWorkingDirectoryPath();
    path.append(Path(TEXT("test")));
    auto file = FileSystem::CreateAndOpenFile(path);

    int32_t len = CString::Strlen(data);
    (*file) << len;
    for (int i = 0; i < len; ++i) {
        (*file) << data[i];
    }
}

TEST(FFileSystemTest, ReadFile) {
    auto path = FileSystem::GetWorkingDirectoryPath();
    path.append(Path(TEXT("test")));
    auto file = FileSystem::OpenFile(path);

    int32_t len = 0;
    (*file) << len;

    TCHAR temp[512];

    for (int i = 0; i < len; ++i) {
        (*file) << temp[i];
    }

    temp[len] = '\0';

    ASSERT_EQ(CString::Strcmp(data, temp), 0);
}

TEST(FFileSystemTest, Delete) {
    FileSystem::Delete(Path(TEXT("~/test")));
}