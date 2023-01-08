#include <gtest/gtest.h>

#include <UtilityCore.h>

TEST(LoggingTest, Log) {
    LOG(LogTemp, Debug, TEXT("Hello, %ls!"), TEXT("Debug!"));
    LOG(LogTemp, Info, TEXT("Hello, %ls!"), TEXT("Info!"));
    LOG(LogTemp, Warning, TEXT("Hello, %ls!"), TEXT("Warning!"));
    LOG(LogTemp, Error, TEXT("Hello, %ls!"), TEXT("Error!"));

    // FATAL is
    // MR_LOG(LogTemp, Fatal, TEXT("Hello, %ls!"), TEXT("Fatal!"));
}
