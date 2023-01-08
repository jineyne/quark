#include <gtest/gtest.h>

#include "Container/Map.h"
#include "Container/String.h"

TEST(MapTest, init) {
    TMap<int, FString> map = {
        { 1, L"Player" },
        { 2, L"Enemy" },
    };

    EXPECT_TRUE(map[1].equals(L"Player"));
    EXPECT_TRUE(map[2].equals(L"Enemy"));
}

TEST(MapTest, assign) {
    TMap<int, FString> map = {
            { 1, L"Player" },
            { 2, L"Enemy" },
    };

    auto other = map;

    EXPECT_TRUE(other[1].equals(L"Player"));
    EXPECT_TRUE(other[2].equals(L"Enemy"));
}

TEST(MapTest, contains) {
    TMap<int, FString> map = {
            { 0, L"Player" },
            { 1, L"Enemy" },
    };

    EXPECT_TRUE(map.contains(1));
    EXPECT_TRUE(map.contains(0));
    EXPECT_FALSE(map.contains(2));
}

TEST(MapTest, find) {
    TMap<int, FString> map = {
            { 1, L"Player" },
            { 2, L"Enemy" },
    };

    EXPECT_TRUE(map.find(1)->equals(L"Player"));
    EXPECT_TRUE(map.find(0) == nullptr);
}