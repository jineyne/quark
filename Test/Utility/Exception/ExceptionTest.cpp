#define USE_SYSTEM_THROW

#include "TestUtility.h"

#include "Exception/Exception.h"


TEST(ExceptionTest, any) {
    ASSERT_ANY_THROW(EXCEPT(LogTemp, InvalidStateException, TEXT("for test :D")));
}