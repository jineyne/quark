#include "ArchiveTest.h"

#define INSERT(TYPE, VALUE) { TYPE __data = VALUE; *(archive) << __data; }
#define ASSERT_DATA(TYPE, EXPECTED) { TYPE __data; *(archive) << __data; ASSERT_EQ(__data, EXPECTED); }
#define ASSERT_DATA_FLOAT(TYPE, EXPECTED, EPSILON) { TYPE __data; *(archive) << __data; ASSERT_NEAR(__data, EXPECTED, EPSILON); }
TEST(FArchiveTest, data) {
    auto memory = MakeShared<FMemoryStream>(1024, EStreamAccessMode::Write);

    {
        FArchive *archive = new FBinaryArchive(memory, EArchiveMode::Save);
        INSERT(int, 10);

        INSERT(bool, false);
        INSERT(int8_t, 123);
        INSERT(int32_t, INT32_MAX);
        INSERT(int64_t, INT64_MAX);

        INSERT(float, 1.0001);
        INSERT(double, 10.0000101011);

        INSERT(FString, TEXT("Hello, Archive!"));
        delete archive;
    }

    memory->seek(0);

    {
        FArchive *archive = new FBinaryArchive(memory, EArchiveMode::Load);
        ASSERT_DATA(int, 10);


        ASSERT_DATA(bool, false);
        ASSERT_DATA(int8_t, 123);
        ASSERT_DATA(int32_t, INT32_MAX);
        ASSERT_DATA(int64_t, INT64_MAX);

        ASSERT_DATA_FLOAT(float, 1.0001, FLT_EPSILON);
        ASSERT_DATA_FLOAT(double, 10.0000101011, DBL_EPSILON);

        ASSERT_DATA(FString, TEXT("Hello, Archive!"));
        delete archive;
    }
}


TEST(FArchiveTest, init) {
    auto path = FFileSystem::GetWorkingDirectoryPath();
    path.append(FPath(TEXT("archived")));

    auto memory = MakeShared<FMemoryStream>(512, EStreamAccessMode::Write);

    {
        FArchive *archive = new FBinaryArchive(memory, EArchiveMode::Save);
        FBaseClass *target = (FBaseClass *) QReflection::InitObject(new FDerivedClass(), FDerivedClass::StaticClass()->getSuperClass(), FDerivedClass::StaticClass(), TEXT("FDerivedClass"), (EObjectFlags) 0);
        target->mParam1 = 1541;
        target->mParam2 = TEXT("Hello, Archive!");
        target->mArrayParam = { TEXT("Hello,"), TEXT("Array!") };
        target->mIntArrayParam = { 1, 5, 4, 1 };
        ((FDerivedClass *) target)->mFloatValue = 3.141592;
        *archive << target;
    }

    memory->seek(0);

    {
        FArchive *archive = new FBinaryArchive(memory, EArchiveMode::Load);
        FBaseClass *target = (FBaseClass *) QReflection::InitObject(new FDerivedClass(), FDerivedClass::StaticClass()->getSuperClass(), FDerivedClass::StaticClass(), TEXT("FDerivedClass"), (EObjectFlags) 0);
        *archive << target;

        ASSERT_EQ(target->mParam1, 1541);
        ASSERT_TRUE(target->mParam2.equals(TEXT("Hello, Archive!")));

        ASSERT_EQ(target->mArrayParam.length(), 2);
        ASSERT_TRUE(target->mArrayParam[0].equals(TEXT("Hello,")));
        ASSERT_TRUE(target->mArrayParam[1].equals(TEXT("Array!")));

        ASSERT_EQ(target->mIntArrayParam.length(), 4);
        ASSERT_TRUE(((FDerivedClass *) target)->mFloatValue - 3.141592 < FLT_EPSILON);
    }
}

int FDerivedClass::dump() {
    return 1;
}
