#include "TestUtility.h"

#include <Reflection/Reflection.h>

enum ETestEnum {
    Value1,
    Value2,
    Value3,
};

QEnum *Generated_Initializer_Enum_ETestEnum();
static QEnum *ETestEnum_StaticEnum();

static QEnum *ETestEnum_StaticEnum() {
    static QEnum *instance = nullptr;
    if (!instance) {
        instance = getStaticEnum(Generated_Initializer_Enum_ETestEnum, TEXT("ETestEnum"));
    }
    return instance;
}

static FInitEnumOnStart Generated_InitEnumOnStart_Enum_ETestEnum(ETestEnum_StaticEnum, TEXT("ETestEnum"), TEXT("EnumTest.cpp"));

static QEnum *Generated_Initializer_Enum_ETestEnum() {
    static QEnum *instance = nullptr;
    if (!instance) {
        static const TArray<QReflection::FEnumEntry> entires = {
                { TEXT("ETestEnum::Value1"), (int64_t) ETestEnum::Value1 },
                { TEXT("ETestEnum::Value2"), (int64_t) ETestEnum::Value2 },
                { TEXT("ETestEnum::Value3"), (int64_t) ETestEnum::Value3 },
        };

        static const TArray<QReflection::FMetaDataPairDesc> metas = {
        };

        static const QReflection::FEnumDesc desc = {
                TEXT("ETestEnum"),
                TEXT("enum"),
                entires,
                metas,
        };

        QReflection::CreateEnum(instance, desc);
    }

    return instance;
}
TEST(EnumTest, LookupEnum) {
    QEnum *enumClass = nullptr;
    QEnum::LookupEnum(TEXT("ETestEnum"), &enumClass);

    ASSERT_TRUE(enumClass);
}

TEST(EnumTest, getValueByName) {
    QEnum *enumClass = nullptr;
    QEnum::LookupEnum(TEXT("ETestEnum"), &enumClass);

    ASSERT_TRUE(enumClass);
    ASSERT_EQ(enumClass->getValueByName(TEXT("Value1")), ETestEnum::Value1);
    ASSERT_EQ(enumClass->getValueByName(TEXT("ETestEnum::Value1")), ETestEnum::Value1);
    ASSERT_EQ(enumClass->getValueByName(TEXT("Value2")), ETestEnum::Value2);
    ASSERT_EQ(enumClass->getValueByName(TEXT("ETestEnum::Value2")), ETestEnum::Value2);
    ASSERT_EQ(enumClass->getValueByName(TEXT("Value3")), ETestEnum::Value3);
    ASSERT_EQ(enumClass->getValueByName(TEXT("ETestEnum::Value3")), ETestEnum::Value3);
}

TEST(EnumTest, getNameByValue) {
    QEnum *enumClass = nullptr;
    QEnum::LookupEnum(TEXT("ETestEnum"), &enumClass);

    ASSERT_TRUE(enumClass);
    ASSERT_TRUE(enumClass->getNameByValue(ETestEnum::Value1).equals(TEXT("ETestEnum::Value1")));
    ASSERT_TRUE(enumClass->getNameByValue(ETestEnum::Value2).equals(TEXT("ETestEnum::Value2")));
    ASSERT_TRUE(enumClass->getNameByValue(ETestEnum::Value3).equals(TEXT("ETestEnum::Value3")));
}

TEST(EnumTest, isValidName) {
    QEnum *enumClass = nullptr;
    QEnum::LookupEnum(TEXT("ETestEnum"), &enumClass);

    ASSERT_TRUE(enumClass);
    ASSERT_TRUE(enumClass->isValidName(TEXT("ETestEnum::Value1")));
    ASSERT_TRUE(enumClass->isValidName(TEXT("Value1")));

    ASSERT_FALSE(enumClass->isValidName(TEXT("ETestEnum::Value4")));
    ASSERT_FALSE(enumClass->isValidName(TEXT("Value4")));

    ASSERT_FALSE(enumClass->isValidName(TEXT("FTestEnum::Value1")));
}

TEST(EnumTest, isValidEnumValue) {
    QEnum *enumClass = nullptr;
    QEnum::LookupEnum(TEXT("ETestEnum"), &enumClass);

    ASSERT_TRUE(enumClass);
    ASSERT_TRUE(enumClass->isValidEnumValue(ETestEnum::Value1));
    ASSERT_TRUE(enumClass->isValidEnumValue(ETestEnum::Value2));
    ASSERT_FALSE(enumClass->isValidEnumValue(ETestEnum::Value2 + 3));
}