#include <gtest/gtest.h>
#include <FileSystem/FileSystem.h>

static TCHAR data[512] = TEXT("Hello, World!\nThis is debug message!");

TEST(FFileSystemTest, WriteFile) {
    auto path = FFileSystem::GetWorkingDirectoryPath();
    path.append(FPath(TEXT("test")));
    auto file = FFileSystem::CreateAndOpenFile(path);

    int32_t len = FCString::Strlen(data);
    (*file) << len;
    for (int i = 0; i < len; ++i) {
        (*file) << data[i];
    }
}

TEST(FFileSystemTest, ReadFile) {
    auto path = FFileSystem::GetWorkingDirectoryPath();
    path.append(FPath(TEXT("test")));
    auto file = FFileSystem::OpenFile(path);

    int32_t len = 0;
    (*file) << len;

    TCHAR temp[512];

    for (int i = 0; i < len; ++i) {
        (*file) << temp[i];
    }

    temp[len] = '\0';

    ASSERT_EQ(FCString::Strcmp(data, temp), 0);
}

TEST(FFileSystemTest, Delete) {
    FFileSystem::Delete(FPath(TEXT("~/test")));
}