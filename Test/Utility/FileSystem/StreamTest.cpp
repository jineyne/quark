#include <gtest/gtest.h>

#include "FileSystem/Stream.h"
#include "FileSystem/FileStream.h"
#include "FileSystem/MemoryStream.h"

static char data[512];

#define INSERT(TYPE, VALUE) { TYPE __value = VALUE; memory << __value; }

TEST(FStreamTest, SerializationTest) {
    auto memory = FMemoryStream(data, 512, EStreamAccessMode::Write);
    INSERT(int8_t, 1);
    INSERT(uint8_t, 2);
    INSERT(int16_t, 32);
    INSERT(uint16_t, 33);
    INSERT(int32_t, 132);
    INSERT(uint32_t, 133);
}

#undef INSERT

#define CHECK(TYPE, EXPECT) { TYPE __value; memory << __value; ASSERT_EQ(__value, EXPECT); }

TEST(FStreamTest, DeserializationTest) {
    auto memory = FMemoryStream(data, 512);

    CHECK(int8_t, 1);
    CHECK(uint8_t, 2);
    CHECK(int16_t, 32);
    CHECK(uint16_t, 33);
    CHECK(int32_t, 132);
    CHECK(uint32_t, 133);
}

#undef CHECK