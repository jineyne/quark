#include "TestUtility.h"

#include <Reflection/Reflection.h>

TEST(ClassTest, superClass) {
    ASSERT_TRUE(ObjectProperty::StaticClass()->getSuperClass() == Property::StaticClass());
    ASSERT_TRUE(StructProperty::StaticClass()->getSuperClass() == ObjectProperty::StaticClass());
}

TEST(ClassTest, thisClass) {
    ASSERT_TRUE(NumbericProperty::StaticClass()->isA<Property>());
    ASSERT_TRUE(BoolProperty::StaticClass()->isA<Property>());

    ASSERT_TRUE(ObjectProperty::StaticClass()->isA<Property>());
    ASSERT_TRUE(ClassProperty::StaticClass()->isA<Property>());
    ASSERT_TRUE(ArrayProperty::StaticClass()->isA<Property>());
}