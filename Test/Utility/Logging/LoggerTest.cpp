#include "TestUtility.h"

#include <UtilityCore.h>

TEST(LoggingTest, Log) {
    LOG(LogTest, Debug, TEXT("Hello, %ls!"), TEXT("Debug!"));
    LOG(LogTest, Info, TEXT("Hello, %ls!"), TEXT("Info!"));
    LOG(LogTest, Warning, TEXT("Hello, %ls!"), TEXT("Warning!"));
    LOG(LogTest, Error, TEXT("Hello, %ls!"), TEXT("Error!"));

    // FATAL is
    // MR_LOG(FLogTemp, Fatal, TEXT("Hello, %ls!"), TEXT("Fatal!"));
}
