#include "TestUtility.h"

#include <Reflection/Reflection.h>

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Test_Utility_Reflection_ClassTest_cpp

#define Test_Utility_Reflection_ClassTest_cpp_19_GENERATED_BODY \
    private:\
        friend struct Generated_Class_ClassTest_Statics; \
        static void StaticRegisterNativeClassTest(); \
    public:\
        DECLARE_CLASS(ClassTest, Object, NO_API)              \
        DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ClassTest) \
        DECLARE_SERIALIZER(ClassTest)

class ClassTest : public Object {
private:
    GENERATED_BODY()

private:
    int mParam1 = 1;
    String mParam2 = 2;

    TArray<int> mParam3;

public:
    ClassTest() {
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

    const String &getParam2() const {
        return mParam2;
    }

    void setParam2(const String &param2) {
        mParam2 = param2;
    }

    const TArray<int> &getParam3() const {
        return mParam3;
    }

    void setMParam3(const TArray<int> &param3) {
        mParam3 = param3;
    }
};

void ClassTest::StaticRegisterNativeClassTest() {
    // FUNCTIONS DECLARE
}

IMPLEMENT_CLASS(ClassTest);

Class *Generated_Initializer_Class_ClassTest();

static InitClassOnStart Generated_InitClassOnStart_Class_ClassTest(&Generated_Initializer_Class_ClassTest, &ClassTest::StaticClass, "ClassTest", "ClassTest.cpp");

struct Generated_Class_ClassTest_Statics {
    static const Reflection::GenericPropertyDesc mParam1_PropertyDesc;
    static const TArray<Reflection::MetaDataPairDesc> mParam1_MetaData;
    static const Reflection::GenericPropertyDesc mParam2_PropertyDesc;
    static const TArray<Reflection::MetaDataPairDesc> mParam2_MetaData;
    static const Reflection::GenericPropertyDesc mParam3_PropertyDesc;
    static const TArray<Reflection::MetaDataPairDesc> mParam3_MetaData;

    static const TArray<Reflection::MetaDataPairDesc> ClassMetaData;
    static const TArray<Reflection::PropertyDescBase const*> ClassProperties;
    static const Reflection::ClassDesc ClassDesc;
};

const Reflection::GenericPropertyDesc Generated_Class_ClassTest_Statics::mParam1_PropertyDesc = {
        TEXT("mParam1"),
        EPropertyFlags::PropertyFlags_Private,
        Reflection::EPropertyGenFlags::Int32,
        sizeof(ClassTest::mParam1),
        1,
        offsetof(ClassTest, mParam1),
        Generated_Class_ClassTest_Statics::mParam1_MetaData,
};
const TArray<Reflection::MetaDataPairDesc> Generated_Class_ClassTest_Statics::mParam1_MetaData = {
};

const Reflection::GenericPropertyDesc Generated_Class_ClassTest_Statics::mParam2_PropertyDesc = {
        TEXT("mParam2"),
        EPropertyFlags::PropertyFlags_Private,
        Reflection::EPropertyGenFlags::String,
        sizeof(ClassTest::mParam2),
        1,
        offsetof(ClassTest, mParam2),
        Generated_Class_ClassTest_Statics::mParam2_MetaData,
};
const TArray<Reflection::MetaDataPairDesc> Generated_Class_ClassTest_Statics::mParam2_MetaData = {
};

const Reflection::GenericPropertyDesc Generated_Class_ClassTest_Statics::mParam3_PropertyDesc = {
        TEXT("mParam3"),
        EPropertyFlags::PropertyFlags_Private,
        Reflection::EPropertyGenFlags::Array,
        sizeof(ClassTest::mParam3),
        1,
        offsetof(ClassTest, mParam2),
        Generated_Class_ClassTest_Statics::mParam3_MetaData,
};
const TArray<Reflection::MetaDataPairDesc> Generated_Class_ClassTest_Statics::mParam3_MetaData = {
};

const TArray<Reflection::MetaDataPairDesc> Generated_Class_ClassTest_Statics::ClassMetaData = {
};
const TArray<Reflection::PropertyDescBase const*> Generated_Class_ClassTest_Statics::ClassProperties = {
        (const Reflection::PropertyDescBase *)&Generated_Class_ClassTest_Statics::mParam1_PropertyDesc,
        (const Reflection::PropertyDescBase *)&Generated_Class_ClassTest_Statics::mParam2_PropertyDesc,
        (const Reflection::PropertyDescBase *)&Generated_Class_ClassTest_Statics::mParam3_PropertyDesc,
};
const Reflection::ClassDesc Generated_Class_ClassTest_Statics::ClassDesc = {
        TEXT("ClassTest"),
        &ClassTest::StaticClass,
        (EClassFlags) 0x0,
        Generated_Class_ClassTest_Statics::ClassProperties,
        Generated_Class_ClassTest_Statics::ClassMetaData,
};

Class *Generated_Initializer_Class_ClassTest() {
    static Class *instance = nullptr;
    if (!instance) {
        Reflection::CreateClass(instance, Generated_Class_ClassTest_Statics::ClassDesc);
    }
    return instance;
}

struct FClassDebugInfo {
    String name;
    size_t size;
};

FClassDebugInfo infos[] = {
        { TEXT("mParam1"), sizeof(int) },
        { TEXT("mParam2"), sizeof(String), },
        { TEXT("mParam3"), sizeof(TArray<int>) },
};

TEST(ClassTest, isA) {
    ASSERT_TRUE(ClassTest::StaticClass()->isA<Object>());
    ASSERT_FALSE(ClassTest::StaticClass()->isA<Class>());
}

TEST(ClassTest, memberVariables) {
    Class *clazz = ClassTest::StaticClass();

    ASSERT_TRUE(clazz->getName().equals(TEXT("ClassTest")));
    ASSERT_EQ(clazz->getCppProperties().length(), 3);

    for (int i = 0; i < clazz->getCppProperties().length(); i++) {
        auto member = clazz->getCppProperties()[i];
        auto info = infos[i];

        ASSERT_TRUE(member->isA<Property>());
        auto property = (Property *) member;


        ASSERT_TRUE(property->getName().equals(info.name));
        ASSERT_TRUE(property->getSize() == info.size);
    }
}

TEST(ClassTest, getset) {
    Class *clazz = ClassTest::StaticClass();

    ClassTest *clazzTest = q_new<ClassTest>();
    clazzTest->setParam1(10);
    clazzTest->setParam2(TEXT("Hello, Reflection"));

    Field *field1 = clazz->getCppPropertiesByName(TEXT("mParam1"));
    ASSERT_TRUE(field1);
    ASSERT_TRUE(field1->isA<Property>());

    Property *property1 = (Property *) field1;
    ASSERT_EQ(*property1->getValuePtr<int>(clazzTest), 10);

    int value = 100;
    property1->setValuePtr(clazzTest, value);
    ASSERT_EQ(clazzTest->getParam1(), 100);

    Field *field2 = clazz->getCppPropertiesByName(TEXT("mParam2"));
    ASSERT_TRUE(field2);
    ASSERT_TRUE(field2->isA<Property>());

    Property *property2 = (Property *) field2;
    ASSERT_TRUE(property2->getValuePtr<String>(clazzTest)->equals(TEXT("Hello, Reflection")));

    String value2 = TEXT("Hello, World!");
    property2->setValuePtr(clazzTest, value2);
    ASSERT_TRUE(clazzTest->getParam2().equals(TEXT("Hello, World!")));
}