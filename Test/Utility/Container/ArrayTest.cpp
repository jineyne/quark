#include <gtest/gtest.h>

#include <Container/Array.h>
#include <Container/String.h>

TEST(ArrayTest, init) {
    TArray<int> array;
    array.init(10, 2);

    EXPECT_EQ(array.length(), 2);
    EXPECT_EQ(array[0], 10);
    EXPECT_EQ(array[1], 10);
}

TEST(ArrayTest, add) {
    TArray<int> array;
    array.add(1);

    EXPECT_EQ(array.length(), 1);
    EXPECT_EQ(array[0], 1);

    array.add(2);
    EXPECT_EQ(array.length(), 2);
    EXPECT_EQ(array[1], 2);
}

TEST(ArrayTest, append) {
    TArray<int> other = { 1, 2, 3 };

    TArray<int> array = { 4, 5 };
    array.append(other);

    EXPECT_EQ(array.length(), 5);
    EXPECT_EQ(array[2], 1);
}

TEST(ArrayTest, find) {
    TArray<FString> array { "this is first", "this is second" };

    EXPECT_EQ(array.find("this is second"), 1);
}

TEST(ArrayTest, insert) {
    TArray<int> other = { 1, 2, 3 };

    TArray<int> array = { 4, 5 };
    array.insert(other, 1);

    EXPECT_EQ(array.length(), 5);
    EXPECT_EQ(array[0], 4);
    EXPECT_EQ(array[1], 1);
    EXPECT_EQ(array[2], 2);
    EXPECT_EQ(array[3], 3);
    EXPECT_EQ(array[4], 5);


    array.insert(10, 2);
    EXPECT_EQ(array[2], 10);
}

TEST(ArrayTest, remove) {
    TArray<int> array = { 1, 2, 3 };
    array.remove(2);

    EXPECT_EQ(array.length(), 2);
    EXPECT_EQ(array[0], 1);
    EXPECT_EQ(array[1], 3);
}

TEST(ArrayTest, swap) {
    TArray<int> array { 1, 2, 3 };
    TArray<int> other;

    EXPECT_NO_THROW(array.swap(other));
    EXPECT_EQ(array.length(), 0);
    EXPECT_EQ(other.length(), 3);
}

TEST(ArrayTest, deepCopy) {
    
    TArray<FString> other;

    {
        TArray<FString> array { TEXT("a"), TEXT("b"), TEXT("c") };
        other = array;
    }

    EXPECT_EQ(other.length(), 3);
}