#include "ArchiveTest.h"
#include "Serialization/TextArchive.h"
#include "Serialization/Formatter/BinaryArchiveFormatter.h"
#include "Serialization/Formatter/YamlArchiveFormatter.h"

#define INSERT(TYPE, VALUE) { TYPE __data = VALUE; *(archive) << __data; }
#define ASSERT_DATA(TYPE, EXPECTED) { TYPE __data; *(archive) << __data; ASSERT_EQ(__data, EXPECTED); }
#define ASSERT_DATA_FLOAT(TYPE, EXPECTED, EPSILON) { TYPE __data; *(archive) << __data; ASSERT_NEAR(__data, EXPECTED, EPSILON); }

TEST(FArchiveTest, MemoryStream) {
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

inline void memset64(void* buffer, int64_t value, size_t count)
{
    const size_t m = count / 8;
    int64_t* p = (int64_t*)buffer;
    for (size_t i = 0; i < m; ++i, ++p)
        *p = value;
}

TEST(FArchiveTest, BinaryArchiveFormatter) {
    auto path = FileSystem::GetWorkingDirectoryPath();
    path.append(Path(TEXT("archived")));

    {
        // TEST:
        std::list<TPair<String, int>> a = {
                { TEXT("Korea"), 82 },
                { TEXT("SomeWhere"), 64 }
        };

        void *temp = &a;

        /*auto &b = *(std::list<uint8_t> *) temp;
        b.push_back(0);
        uint8_t *raw = &b.back();
        TPair<String, int> pair1 = { TEXT("Afghanistan"), 93 };
        memcpy(raw, &pair1, sizeof(TPair<String, int>));

        b.push_back(0);
        raw = &b.back();
        TPair<String, int> pair2 = { TEXT("Albania"), 355 };
        memcpy(raw, &pair2, sizeof(TPair<String, int>));

        b.push_back(0);
        raw = &b.back();
        TPair<String, int> pair3 = { TEXT("Algeria"), 213 };
        memcpy(raw, &pair3, sizeof(TPair<String, int>));

        b.push_back(1); */
    }

    auto memory = MakeShared<MemoryStream>(512, EStreamAccessMode::Write);

    {
        BaseClass *target = (BaseClass *) newObject<DerivedClass>();
        target->mParam1 = 1541;
        target->mParam2 = TEXT("Hello, Archive!");
        target->mArray = { TEXT("Hello,"), TEXT("Array!") };
        target->mSaveDataList = { { 1, 100 }, { 2, 200 } };
        ((DerivedClass *) target)->mFloatValue = 3.141592;

        OtherData *data = newObject<OtherData>(target);
        data->setFoo(512);
        data->setBar(123);

        DerivedClass *derived = (DerivedClass *) target;

        derived->mOtherDataList.add(data);
        derived->mStringToIntMap[TEXT("Korea")] = 82;
        derived->mStringToIntMap[TEXT("Hong Kong")] = 852;

        auto archive = MakeShared<BinaryArchive>(memory, EArchiveMode::Save);
        BinaryArchiveFormatter formatter(*archive);
        formatter.serialize((Object *&) target);
    }

    memory->seek(0);

    {
        Archive *archive = q_new<BinaryArchive>(memory, EArchiveMode::Load);
        BinaryArchiveFormatter formatter(*archive);

        BaseClass *target = (BaseClass *) newObject<DerivedClass>();
        formatter.serialize((Object *&) target);

        ASSERT_EQ(target->mParam1, 1541);
        ASSERT_TRUE(target->mParam2.equals(TEXT("Hello, Archive!")));

        ASSERT_EQ(target->mArray.length(), 2);
        ASSERT_TRUE(target->mArray[0].equals(TEXT("Hello,")));
        ASSERT_TRUE(target->mArray[1].equals(TEXT("Array!")));

        ASSERT_EQ(target->mSaveDataList.length(), 2);
        ASSERT_EQ(target->mSaveDataList[0].level, 1);
        ASSERT_EQ(target->mSaveDataList[0].coin, 100);
        ASSERT_EQ(target->mSaveDataList[1].level, 2);
        ASSERT_EQ(target->mSaveDataList[1].coin, 200);

        DerivedClass *derived = (DerivedClass *) target;
        ASSERT_EQ(derived->mOtherDataList.length(), 1);
        ASSERT_EQ(derived->mOtherDataList[0]->getFoo(), 512);
        ASSERT_EQ(derived->mOtherDataList[0]->getBar(), 123);

        ASSERT_EQ(derived->mStringToIntMap.length(), 2);
        ASSERT_EQ(derived->mStringToIntMap["Korea"], 82);
        ASSERT_EQ(derived->mStringToIntMap["Hong Kong"], 852);

        ASSERT_TRUE(derived->mFloatValue - 3.141592 < FLT_EPSILON);
    }
}

// TODO: FTestArchive is not maintain
/*TEST(FArchiveTest, TextArchive) {
    auto path = FileSystem::GetWorkingDirectoryPath();
    path.append(Path(TEXT("archived")));

    auto memory = MakeShared<MemoryStream>(1024, EStreamAccessMode::Write);

    {
        BaseClass *target = (BaseClass *) newObject<DerivedClass>();
        target->mParam1 = 1541;
        target->mParam2 = TEXT("Hello, Archive!");
        target->mArray = {TEXT("Hello,"), TEXT("Array!")};
        target->mSaveDataList = {{1, 100}, {2, 200}};
        ((DerivedClass *) target)->mFloatValue = 3.141592;

        OtherData *data = newObject<OtherData>(target);
        data->setFoo(512);
        data->setBar(123);

        ((DerivedClass *) target)->mOtherDataList.add(data);

        auto fileArchive = MakeShared<TextArchive>(memory, EArchiveMode::Save);
        *(fileArchive.get()) << target;
    }
}*/

TEST(FArchiveTest, YamlArchiveFormatter) {
    auto path = FileSystem::GetWorkingDirectoryPath();
    path.append(Path(TEXT("archived")));

    auto memory = MakeShared<MemoryStream>(1024, EStreamAccessMode::Write);

    {
        BaseClass *target = (BaseClass *) newObject<DerivedClass>();
        target->mParam1 = 1541;
        target->mParam2 = TEXT("Hello, Archive!");
        target->mArray = {TEXT("Hello,"), TEXT("Array!")};
        target->mSaveDataList = {{1, 100}, {2, 200}};
        ((DerivedClass *) target)->mFloatValue = 3.141592;

        OtherData *data = newObject<OtherData>(target);
        data->setFoo(512);
        data->setBar(123);

        ((DerivedClass *) target)->mOtherDataList.add(data);
        ((DerivedClass *) target)->mStringToIntMap[TEXT("Korea")] = 82;
        ((DerivedClass *) target)->mStringToIntMap[TEXT("Hong Kong")] = 852;

        auto archive = q_new<BinaryArchive>(memory, EArchiveMode::Save);
        YamlArchiveFormatter formatter(*archive);
        formatter.serialize((Object *&) target);
    }

    memory->seek(0);

    {
        Archive *archive = q_new<BinaryArchive>(memory, EArchiveMode::Load);
        YamlArchiveFormatter formatter(*archive);

        BaseClass *target = (BaseClass *) newObject<DerivedClass>();
        formatter.serialize((Object *&) target);

        ASSERT_EQ(target->mParam1, 1541);
        ASSERT_TRUE(target->mParam2.equals(TEXT("Hello, Archive!")));

        ASSERT_EQ(target->mArray.length(), 2);
        ASSERT_TRUE(target->mArray[0].equals(TEXT("Hello,")));
        ASSERT_TRUE(target->mArray[1].equals(TEXT("Array!")));

        ASSERT_EQ(target->mSaveDataList.length(), 2);
        ASSERT_EQ(target->mSaveDataList[0].level, 1);
        ASSERT_EQ(target->mSaveDataList[0].coin, 100);

        ASSERT_EQ(((DerivedClass *) target)->mOtherDataList.length(), 1);
        ASSERT_EQ(((DerivedClass *) target)->mOtherDataList[0]->getFoo(), 512);
        ASSERT_EQ(((DerivedClass *) target)->mOtherDataList[0]->getBar(), 123);

        ASSERT_EQ(((DerivedClass *) target)->mStringToIntMap.length(), 2);
        ASSERT_EQ(((DerivedClass *) target)->mStringToIntMap["Korea"], 82);
        ASSERT_EQ(((DerivedClass *) target)->mStringToIntMap["Hong Kong"], 852);

        ASSERT_TRUE(((DerivedClass *) target)->mFloatValue - 3.141592 < FLT_EPSILON);
    }
}

int DerivedClass::dump() {
    return 1;
}