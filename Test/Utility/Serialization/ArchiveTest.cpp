/*
#include "ArchiveTest.h"
#include "Serialization/TextArchive.h"

#define INSERT(TYPE, VALUE) { TYPE __data = VALUE; *(archive) << __data; }
#define ASSERT_DATA(TYPE, EXPECTED) { TYPE __data; *(archive) << __data; ASSERT_EQ(__data, EXPECTED); }
#define ASSERT_DATA_FLOAT(TYPE, EXPECTED, EPSILON) { TYPE __data; *(archive) << __data; ASSERT_NEAR(__data, EXPECTED, EPSILON); }
TEST(FArchiveTest, data) {
    auto memory = MakeShared<MemoryStream>(1024, EStreamAccessMode::Write);

    {
        Archive *archive = q_new<BinaryArchive>(memory, EArchiveMode::Save);
        INSERT(int, 10);

        INSERT(bool, false);
        INSERT(int8_t, 123);
        INSERT(int32_t, INT32_MAX);
        INSERT(int64_t, INT64_MAX);

        INSERT(float, 1.0001);
        INSERT(double, 10.0000101011);

        INSERT(String, TEXT("Hello, Archive!"));
        q_delete(archive);
    }

    memory->seek(0);

    {
        Archive *archive = q_new<BinaryArchive>(memory, EArchiveMode::Load);
        ASSERT_DATA(int, 10);


        ASSERT_DATA(bool, false);
        ASSERT_DATA(int8_t, 123);
        ASSERT_DATA(int32_t, INT32_MAX);
        ASSERT_DATA(int64_t, INT64_MAX);

        ASSERT_DATA_FLOAT(float, 1.0001, FLT_EPSILON);
        ASSERT_DATA_FLOAT(double, 10.0000101011, DBL_EPSILON);

        ASSERT_DATA(String, TEXT("Hello, Archive!"));
        q_delete(archive);
    }
}


TEST(FArchiveTest, FMemoryStream) {
    auto path = FileSystem::GetWorkingDirectoryPath();
    path.append(Path(TEXT("archived")));

    auto memory = MakeShared<MemoryStream>(512, EStreamAccessMode::Write);

    {
        FBaseClass *target = (FBaseClass *) newObject<FDerivedClass>();
        target->mParam1 = 1541;
        target->mParam2 = TEXT("Hello, Archive!");
        target->mArray = { TEXT("Hello,"), TEXT("Array!") };
        target->mSaveDataList = {{ 1, 100 }, { 2, 200 } };
        ((FDerivedClass *) target)->mFloatValue = 3.141592;

        QOtherData *data = newObject<QOtherData>(target);
        data->setFoo(512);
        data->setBar(123);

        ((FDerivedClass *) target)->mOtherDataList.add(data);

        auto archive = MakeShared<BinaryArchive>(memory, EArchiveMode::Save);
        *(archive.get()) << target;
    }

    memory->seek(0);

    {
        Archive *archive = q_new<BinaryArchive>(memory, EArchiveMode::Load);
        FBaseClass *target = (FBaseClass *) newObject<FDerivedClass>();

        *archive << target;

        ASSERT_EQ(target->mParam1, 1541);
        ASSERT_TRUE(target->mParam2.equals(TEXT("Hello, Archive!")));

        ASSERT_EQ(target->mArray.length(), 2);
        ASSERT_TRUE(target->mArray[0].equals(TEXT("Hello,")));
        ASSERT_TRUE(target->mArray[1].equals(TEXT("Array!")));

        ASSERT_EQ(target->mSaveDataList.length(), 2);
        ASSERT_EQ(target->mSaveDataList[0].level, 1);
        ASSERT_EQ(target->mSaveDataList[0].coin, 100);

        ASSERT_EQ(((FDerivedClass *) target)->mOtherDataList.length(), 1);
        ASSERT_EQ(((FDerivedClass *) target)->mOtherDataList[0]->getFoo(), 512);
        ASSERT_EQ(((FDerivedClass *) target)->mOtherDataList[0]->getBar(), 123);

        ASSERT_TRUE(((FDerivedClass *) target)->mFloatValue - 3.141592 < FLT_EPSILON);
    }
}

// TODO: FTestArchive is not maintain
TEST(FArchiveTest, FTextArchive) {
    auto path = FileSystem::GetWorkingDirectoryPath();
    path.append(Path(TEXT("archived")));

    auto memory = MakeShared<MemoryStream>(1024, EStreamAccessMode::Write);

    {
        FBaseClass *target = (FBaseClass *) newObject<FDerivedClass>();
        target->mParam1 = 1541;
        target->mParam2 = TEXT("Hello, Archive!");
        target->mArray = { TEXT("Hello,"), TEXT("Array!") };
        target->mSaveDataList = {{ 1, 100 }, { 2, 200 } };
        ((FDerivedClass *) target)->mFloatValue = 3.141592;

        QOtherData *data = newObject<QOtherData>(target);
        data->setFoo(512);
        data->setBar(123);

        ((FDerivedClass *) target)->mOtherDataList.add(data);

        auto fileArchive = MakeShared<TextArchive>(memory, EArchiveMode::Save);
        *(fileArchive.get()) << target;
    }

    memory->seek(0);

    */
/*{
        Archive *archive = new TextArchive(memory, EArchiveMode::Load);
        FBaseClass *target = (FBaseClass *) newObject<FDerivedClass>();

        *archive << target;

        ASSERT_EQ(target->mParam1, 1541);
        ASSERT_TRUE(target->mParam2.equals(TEXT("Hello, Archive!")));

        ASSERT_EQ(target->mArray.length(), 2);
        ASSERT_TRUE(target->mArray[0].equals(TEXT("Hello,")));
        ASSERT_TRUE(target->mArray[1].equals(TEXT("Array!")));

        ASSERT_EQ(target->mSaveDataList.length(), 2);
        ASSERT_EQ(target->mSaveDataList[0].level, 1);
        ASSERT_EQ(target->mSaveDataList[0].coin, 100);

        ASSERT_EQ(((FDerivedClass *) target)->mOtherDataList.length(), 1);
        ASSERT_EQ(((FDerivedClass *) target)->mOtherDataList[0]->getFoo(), 512);
        ASSERT_EQ(((FDerivedClass *) target)->mOtherDataList[0]->getBar(), 123);

        ASSERT_TRUE(((FDerivedClass *) target)->mFloatValue - 3.141592 < FLT_EPSILON);
    }*//*

}


int FDerivedClass::dump() {
    return 1;
}
*/
