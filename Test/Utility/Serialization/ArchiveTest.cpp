#include "TestUtility.h"

#include "Serialization/Archive.h"
#include "FileSystem/FileSystem.h"
#include "FileSystem/MemoryStream.h"
#include <Reflection/Reflection.h>

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Test_Utility_Serialization_ArchiveTest_cpp

#define Test_Utility_Serialization_ArchiveTest_cpp_21_GENERATED_BODY \
    private:\
        friend struct Generated_Class_FArchiveTest_Statics; \
        static void StaticRegisterNativeFArchiveTest(); \
    public:\
        DECLARE_CLASS(FArchiveTest, QObject, NO_API) \
        DECLARE_SERIALIZER(FArchiveTest)

class FArchiveTest : public QObject {
private:
    GENERATED_BODY()

private:
    int mParam1 = 1;
    FString mParam2 = 2;

public:
    FArchiveTest() {
        //
    }

public:
    int getParam1() const {
        return mParam1;
    }

    void setParam1(int param1) {
        mParam1 = param1;
    }

    const FString &getParam2() const {
        return mParam2;
    }

    void setParam2(const FString &param2) {
        mParam2 = param2;
    }
};

void FArchiveTest::StaticRegisterNativeFArchiveTest() {
    // FUNCTIONS DECLARE
}

IMPLEMENT_CLASS(FArchiveTest);

QClass *Generated_Initializer_Class_FArchiveTest();

static FInitClassOnStart Generated_InitClassOnStart_Class_FArchiveTest(&Generated_Initializer_Class_FArchiveTest, &FArchiveTest::StaticClass, "FArchiveTest", "ArchiveTest.cpp");

struct Generated_Class_FArchiveTest_Statics {
    static const QReflection::FArrayPropertyDesc mParam1_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mParam1_MetaData;
    static const QReflection::FGenericPropertyDesc mParam2_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mParam2_MetaData;

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const QReflection::FArrayPropertyDesc Generated_Class_FArchiveTest_Statics::mParam1_PropertyDesc = {
        "mParam1",
        EPropertyFlags::PropertyFlags_Private,
        QReflection::EPropertyGenFlags::Int32,
        sizeof(FArchiveTest::mParam1),
        1,
        offsetof(FArchiveTest, mParam1),
        Generated_Class_FArchiveTest_Statics::mParam1_MetaData,
};
const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FArchiveTest_Statics::mParam1_MetaData = {
};

const QReflection::FGenericPropertyDesc Generated_Class_FArchiveTest_Statics::mParam2_PropertyDesc = {
        "mParam2",
        EPropertyFlags::PropertyFlags_Private,
        QReflection::EPropertyGenFlags::String,
        sizeof(FArchiveTest::mParam2),
        1,
        offsetof(FArchiveTest, mParam2),
        Generated_Class_FArchiveTest_Statics::mParam2_MetaData,
};
const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FArchiveTest_Statics::mParam2_MetaData = {
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FArchiveTest_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FArchiveTest_Statics::ClassProperties = {
        (const QReflection::FPropertyDescBase *)&Generated_Class_FArchiveTest_Statics::mParam1_PropertyDesc,
        (const QReflection::FPropertyDescBase *)&Generated_Class_FArchiveTest_Statics::mParam2_PropertyDesc,
};
const QReflection::FClassDesc Generated_Class_FArchiveTest_Statics::ClassDesc = {
        "FArchiveTest",
        &FArchiveTest::StaticClass,
        (EClassFlags) 0x0,
        Generated_Class_FArchiveTest_Statics::ClassProperties,
        Generated_Class_FArchiveTest_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FArchiveTest() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FArchiveTest_Statics::ClassDesc);
    }
    return instance;
}

#define INSERT(TYPE, VALUE) { TYPE __data = VALUE; archive << __data; }
#define ASSERT_DATA(TYPE, EXPECTED) { TYPE __data; archive << __data; ASSERT_EQ(__data, EXPECTED); }
#define ASSERT_DATA_FLOAT(TYPE, EXPECTED, EPSILON) { TYPE __data; archive << __data; ASSERT_NEAR(__data, EXPECTED, EPSILON); }
TEST(FArchiveTest, data) {
    auto memory = MakeShared<FMemoryStream>(1024, EStreamAccessMode::Write);

    {
        FArchive archive(memory, EArchiveMode::Save);
        INSERT(int, 10);

        INSERT(bool, false);
        INSERT(int8_t, 123);
        INSERT(int32_t, INT32_MAX);
        INSERT(int64_t, INT64_MAX);

        INSERT(float, 1.0001);
        INSERT(double, 10.0000101011);

        INSERT(FString, TEXT("Hello, Archive!"));
    }

    memory->seek(0);

    {
        FArchive archive(memory, EArchiveMode::Load);
        ASSERT_DATA(int, 10);


        ASSERT_DATA(bool, false);
        ASSERT_DATA(int8_t, 123);
        ASSERT_DATA(int32_t, INT32_MAX);
        ASSERT_DATA(int64_t, INT64_MAX);

        ASSERT_DATA_FLOAT(float, 1.0001, FLT_EPSILON);
        ASSERT_DATA_FLOAT(double, 10.0000101011, DBL_EPSILON);

        ASSERT_DATA(FString, TEXT("Hello, Archive!"));
    }
}


TEST(FArchiveTest, init) {
    auto path = FFileSystem::GetWorkingDirectoryPath();
    path.append(FPath(TEXT("archived")));

    {
        auto file = FFileSystem::CreateAndOpenFile(path);
        FArchive archive(file, EArchiveMode::Save);
        FArchiveTest *target = (FArchiveTest *) QReflection::InitObject(new FArchiveTest(), FArchiveTest::StaticClass()->getSuperClass(), FArchiveTest::StaticClass(), TEXT("FArchiveTest"), (EObjectFlags) 0);
        target->setParam1(1541);
        target->setParam2(TEXT("Hello, Archive!"));
        archive << target;
    }

    {
        auto file = FFileSystem::OpenFile(path);
        FArchive archive(file, EArchiveMode::Load);
        FArchiveTest *target = (FArchiveTest *) QReflection::InitObject(new FArchiveTest(), FArchiveTest::StaticClass()->getSuperClass(), FArchiveTest::StaticClass(), TEXT("FArchiveTest"), (EObjectFlags) 0);
        archive << target;

        ASSERT_EQ(target->getParam1(), 1541);
        ASSERT_TRUE(target->getParam2().equals(TEXT("Hello, Archive!")));
    }
}