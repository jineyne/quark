#include "TestUtility.h"

#include <Reflection/Reflection.h>

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Test_Utility_Reflection_ClassTest_cpp

#define Test_Utility_Reflection_ClassTest_cpp_18_GENERATED_BODY \
    private:\
        friend struct Generated_Class_FClassTest_Statics; \
        static void StaticRegisterNativeFClassTest(); \
    public:\
        DECLARE_CLASS(FClassTest, QObject, NO_API) \
        DECLARE_SERIALIZER(FClassTest)

class FClassTest : public QObject {
private:
    GENERATED_BODY()

private:
    int mParam1 = 1;
    FString mParam2 = 2;

    TArray<int> mParam3;

public:
    FClassTest() {
        //
    }

private:
    int foo() {
        LOG(LogTest, Debug, TEXT("foo called"));
        return 1;
    }

    void bar() {
        LOG(LogTest, Debug, TEXT("foo called"));
        throw "Test";
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

    const TArray<int> &getParam3() const {
        return mParam3;
    }

    void setMParam3(const TArray<int> &param3) {
        mParam3 = param3;
    }
};

void FClassTest::StaticRegisterNativeFClassTest() {
    // FUNCTIONS DECLARE
}

IMPLEMENT_CLASS(FClassTest);

QClass *Generated_Initializer_Class_FClassTest();

static FInitClassOnStart Generated_InitClassOnStart_Class_FClassTest(&Generated_Initializer_Class_FClassTest, &FClassTest::StaticClass, "FClassTest", "ClassTest.cpp");

struct Generated_Class_FClassTest_Statics {
    static const QReflection::FGenericPropertyDesc mParam1_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mParam1_MetaData;
    static const QReflection::FGenericPropertyDesc mParam2_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mParam2_MetaData;
    static const QReflection::FGenericPropertyDesc mParam3_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mParam3_MetaData;

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const QReflection::FGenericPropertyDesc Generated_Class_FClassTest_Statics::mParam1_PropertyDesc = {
        "mParam1",
        EPropertyFlags::PropertyFlags_Private,
        QReflection::EPropertyGenFlags::Int32,
        sizeof(FClassTest::mParam1),
        1,
        offsetof(FClassTest, mParam1),
        Generated_Class_FClassTest_Statics::mParam1_MetaData,
};
const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FClassTest_Statics::mParam1_MetaData = {
};

const QReflection::FGenericPropertyDesc Generated_Class_FClassTest_Statics::mParam2_PropertyDesc = {
        "mParam2",
        EPropertyFlags::PropertyFlags_Private,
        QReflection::EPropertyGenFlags::String,
        sizeof(FClassTest::mParam2),
        1,
        offsetof(FClassTest, mParam2),
        Generated_Class_FClassTest_Statics::mParam2_MetaData,
};
const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FClassTest_Statics::mParam2_MetaData = {
};

const QReflection::FGenericPropertyDesc Generated_Class_FClassTest_Statics::mParam3_PropertyDesc = {
        "mParam3",
        EPropertyFlags::PropertyFlags_Private,
        QReflection::EPropertyGenFlags::Array,
        sizeof(FClassTest::mParam3),
        1,
        offsetof(FClassTest, mParam2),
        Generated_Class_FClassTest_Statics::mParam3_MetaData,
};
const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FClassTest_Statics::mParam3_MetaData = {
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FClassTest_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FClassTest_Statics::ClassProperties = {
        (const QReflection::FPropertyDescBase *)&Generated_Class_FClassTest_Statics::mParam1_PropertyDesc,
        (const QReflection::FPropertyDescBase *)&Generated_Class_FClassTest_Statics::mParam2_PropertyDesc,
        (const QReflection::FPropertyDescBase *)&Generated_Class_FClassTest_Statics::mParam3_PropertyDesc,
};
const QReflection::FClassDesc Generated_Class_FClassTest_Statics::ClassDesc = {
        "FClassTest",
        &FClassTest::StaticClass,
        (EClassFlags) 0x0,
        Generated_Class_FClassTest_Statics::ClassProperties,
        Generated_Class_FClassTest_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FClassTest() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FClassTest_Statics::ClassDesc);
    }
    return instance;
}

struct FClassDebugInfo {
    FString name;
    size_t size;
};

FClassDebugInfo infos[] = {
        { TEXT("mParam1"), sizeof(int) },
        { TEXT("mParam2"), sizeof(FString), },
        { TEXT("mParam3"), sizeof(TArray<int>) },
};

TEST(ClassTest, isA) {
    ASSERT_TRUE(FClassTest::StaticClass()->isA<QObject>());
    ASSERT_FALSE(FClassTest::StaticClass()->isA<QClass>());
}

TEST(ClassTest, memberVariables) {
    QClass *clazz = FClassTest::StaticClass();

    ASSERT_TRUE(clazz->getName().equals(TEXT("FClassTest")));
    ASSERT_EQ(clazz->getCppProperties().length(), 3);

    for (int i = 0; i < clazz->getCppProperties().length(); i++) {
        auto member = clazz->getCppProperties()[i];
        auto info = infos[i];

        ASSERT_TRUE(member->isA<QProperty>());
        auto property = (QProperty *) member;


        ASSERT_TRUE(property->getName().equals(info.name));
        ASSERT_TRUE(property->getSize() == info.size);
    }
}

TEST(ClassTest, getset) {
    QClass *clazz = FClassTest::StaticClass();

    FClassTest *clazzTest = new FClassTest();
    clazzTest->setParam1(10);
    clazzTest->setParam2(TEXT("Hello, Reflection"));

    QField *field1 = clazz->getCppPropertiesByName(TEXT("mParam1"));
    ASSERT_TRUE(field1);
    ASSERT_TRUE(field1->isA<QProperty>());

    QProperty *property1 = (QProperty *) field1;
    ASSERT_EQ(*property1->getValuePtr<int>(clazzTest), 10);

    int value = 100;
    property1->setValuePtr(clazzTest, value);
    ASSERT_EQ(clazzTest->getParam1(), 100);

    QField *field2 = clazz->getCppPropertiesByName(TEXT("mParam2"));
    ASSERT_TRUE(field2);
    ASSERT_TRUE(field2->isA<QProperty>());

    QProperty *property2 = (QProperty *) field2;
    ASSERT_TRUE(property2->getValuePtr<FString>(clazzTest)->equals(TEXT("Hello, Reflection")));

    FString value2 = TEXT("Hello, World!");
    property2->setValuePtr(clazzTest, value2);
    ASSERT_TRUE(clazzTest->getParam2().equals(TEXT("Hello, World!")));
}