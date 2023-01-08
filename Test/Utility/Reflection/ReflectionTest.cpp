#include "TestUtility.h"

#include <Reflection/Reflection.h>

TEST(ClassTest, superClass) {
    ASSERT_TRUE(QObjectProperty::StaticClass()->getSuperClass() == QProperty::StaticClass());
    ASSERT_TRUE(QStructProperty::StaticClass()->getSuperClass() == QObjectProperty::StaticClass());
}

TEST(ClassTest, thisClass) {
    ASSERT_TRUE(QNumbericProperty::StaticClass()->isA<QProperty>());
    ASSERT_TRUE(QBoolProperty::StaticClass()->isA<QProperty>());

    ASSERT_TRUE(QObjectProperty::StaticClass()->isA<QProperty>());
    ASSERT_TRUE(QClassProperty::StaticClass()->isA<QProperty>());
    ASSERT_TRUE(QArrayProperty::StaticClass()->isA<QProperty>());
}