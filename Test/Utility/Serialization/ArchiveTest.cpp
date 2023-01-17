#include "TestUtility.h"

#include "Serialization/Archive.h"
#include "FileSystem/FileSystem.h"
#include "FileSystem/MemoryStream.h"

#include <Reflection/Reflection.h>

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Test_Utility_Serialization_ArchiveTest_cpp

#define Test_Utility_Serialization_ArchiveTest_cpp_22_GENERATED_BODY \
    private:\
        friend struct Generated_Class_FArchiveTest_Statics; \
        static void StaticRegisterNativeFArchiveTest(); \
    public:\
        DECLARE_CLASS(FArchiveTest, QObject, NO_API) \
        DECLARE_SERIALIZER(FArchiveTest)

class FArchiveTest : public QObject {
private:
    GENERATED_BODY()

public:
    int mParam1 = 1;
    FString mParam2 = TEXT("UnInitialized Parameter");

    TArray<FString> mArrayParam;
    TArray<int> mIntArrayParam;

public:
    FArchiveTest() {
        //
    }
};

void FArchiveTest::StaticRegisterNativeFArchiveTest() {
    // FUNCTIONS DECLARE
}

IMPLEMENT_CLASS(FArchiveTest);

QClass *Generated_Initializer_Class_FArchiveTest();

static FInitClassOnStart Generated_InitClassOnStart_Class_FArchiveTest(&Generated_Initializer_Class_FArchiveTest, &FArchiveTest::StaticClass, "FArchiveTest", "ArchiveTest.cpp");

struct Generated_Class_FArchiveTest_Statics {
    static const QReflection::FGenericPropertyDesc mParam1_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mParam1_MetaData;
    static const QReflection::FGenericPropertyDesc mParam2_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mParam2_MetaData;
    static const QReflection::FArrayPropertyDesc mArrayParam_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mArrayParam_MetaData;
    static const QReflection::FArrayPropertyDesc mIntArrayParam_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mIntArrayParam_MetaData;

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const QReflection::FGenericPropertyDesc Generated_Class_FArchiveTest_Statics::mParam1_PropertyDesc = {
        "mParam1",
        EPropertyFlags::PropertyFlags_Public,
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
        EPropertyFlags::PropertyFlags_Public,
        QReflection::EPropertyGenFlags::String,
        sizeof(FArchiveTest::mParam2),
        1,
        offsetof(FArchiveTest, mParam2),
        Generated_Class_FArchiveTest_Statics::mParam2_MetaData,
};
const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FArchiveTest_Statics::mParam2_MetaData = {
};

const QReflection::FArrayPropertyDesc Generated_Class_FArchiveTest_Statics::mArrayParam_PropertyDesc = {
        "mArrayParam",
        EPropertyFlags::PropertyFlags_Public,
        QReflection::EPropertyGenFlags::Array,
        sizeof(FArchiveTest::mArrayParam),
        1,
        offsetof(FArchiveTest, mArrayParam),
        new QStringProperty(nullptr, TEXT("mArrayParam_Template"), 0),
        Generated_Class_FArchiveTest_Statics::mArrayParam_MetaData,
};
const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FArchiveTest_Statics::mArrayParam_MetaData = {
};

const QReflection::FArrayPropertyDesc Generated_Class_FArchiveTest_Statics::mIntArrayParam_PropertyDesc = {
        "mIntArrayParam",
        EPropertyFlags::PropertyFlags_Public,
        QReflection::EPropertyGenFlags::Array,
        sizeof(FArchiveTest::mIntArrayParam),
        1,
        offsetof(FArchiveTest, mIntArrayParam),
        new QIntProperty(nullptr, TEXT("mIntArrayParam_Template"), 0),
        Generated_Class_FArchiveTest_Statics::mIntArrayParam_MetaData,
};
const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FArchiveTest_Statics::mIntArrayParam_MetaData = {
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FArchiveTest_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FArchiveTest_Statics::ClassProperties = {
        (const QReflection::FPropertyDescBase *)&Generated_Class_FArchiveTest_Statics::mParam1_PropertyDesc,
        (const QReflection::FPropertyDescBase *)&Generated_Class_FArchiveTest_Statics::mParam2_PropertyDesc,
        (const QReflection::FPropertyDescBase *)&Generated_Class_FArchiveTest_Statics::mArrayParam_PropertyDesc,
        (const QReflection::FPropertyDescBase *)&Generated_Class_FArchiveTest_Statics::mIntArrayParam_PropertyDesc,
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
        target->mParam1 = 1541;
        target->mParam2 = TEXT("Hello, Archive!");
        target->mArrayParam = { TEXT("Hello,"), TEXT("Array!") };
        target->mIntArrayParam = { 1, 5, 4, 1 };
        archive << target;
    }

    {
        auto file = FFileSystem::OpenFile(path);
        FArchive archive(file, EArchiveMode::Load);
        FArchiveTest *target = (FArchiveTest *) QReflection::InitObject(new FArchiveTest(), FArchiveTest::StaticClass()->getSuperClass(), FArchiveTest::StaticClass(), TEXT("FArchiveTest"), (EObjectFlags) 0);
        archive << target;

        ASSERT_EQ(target->mParam1, 1541);
        ASSERT_TRUE(target->mParam2.equals(TEXT("Hello, Archive!")));

        ASSERT_EQ(target->mArrayParam.length(), 2);
        ASSERT_TRUE(target->mArrayParam[0].equals(TEXT("Hello,")));
        ASSERT_TRUE(target->mArrayParam[1].equals(TEXT("Array!")));

        ASSERT_EQ(target->mIntArrayParam.length(), 4);
    }
}