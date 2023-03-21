#include "UUID.h"
#include "Platform.h"

constexpr const char HEX_TO_LITERAL[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

constexpr const uint8_t LITERAL_TO_HEX[256] = {
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        // 0 through 9 translate to 0  though 9
        0x00,  0x01,  0x02,  0x03,  0x04,  0x05,  0x06,  0x07,  0x08,  0x09,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        // A through F translate to 10 though 15
        0xFF,  0x0A,  0x0B,  0x0C,  0x0D,  0x0E,  0x0F,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        // a through f translate to 10 though 15
        0xFF,  0x0A,  0x0B,  0x0C,  0x0D,  0x0E,  0x0F,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF
};

FUuid FUuid::Empty;

FUuid::FUuid(const FString &uuid) {
    memset(mData, 0, sizeof(mData));

    if (uuid.length() < 36) {
        return;
    }

    uint32_t idx = 0;

    // First group: 8 digits
    for (int32_t i = 7; i >= 0; --i) {
        char charVal = uuid[idx++];
        uint8_t hexVal = LITERAL_TO_HEX[(int)charVal];

        mData[0] |= hexVal << (i * 4);
    }

    idx++;

    // Second group: 4 digits
    for (int32_t i = 7; i >= 4; --i) {
        char charVal = uuid[idx++];
        uint8_t hexVal = LITERAL_TO_HEX[(int)charVal];

        mData[1] |= hexVal << (i * 4);
    }

    idx++;

    // Third group: 4 digits
    for (int32_t i = 3; i >= 0; --i) {
        char charVal = uuid[idx++];
        uint8_t hexVal = LITERAL_TO_HEX[(int)charVal];

        mData[1] |= hexVal << (i * 4);
    }

    idx++;

    // Fourth group: 4 digits
    for (int32_t i = 7; i >= 4; --i) {
        char charVal = uuid[idx++];
        uint8_t hexVal = LITERAL_TO_HEX[(int)charVal];

        mData[2] |= hexVal << (i * 4);
    }

    idx++;

    // Fifth group: 12 digits
    for (int32_t i = 3; i >= 0; --i) {
        char charVal = uuid[idx++];
        uint8_t hexVal = LITERAL_TO_HEX[(int)charVal];

        mData[2] |= hexVal << (i * 4);
    }

    for (int32_t i = 7; i >= 0; --i) {
        char charVal = uuid[idx++];
        uint8_t hexVal = LITERAL_TO_HEX[(int)charVal];

        mData[3] |= hexVal << (i * 4);
    }
}

FString FUuid::toString() const {
    TArray<TCHAR> output(36);
    uint32_t idx = 0;

    // First group: 8 digits
    for (int32_t i = 7; i >= 0; --i) {
        uint32_t hexVal = (mData[0] >> (i * 4)) & 0xF;
        output[idx++] = HEX_TO_LITERAL[hexVal];
    }

    output[idx++] = '-';

    // Second group: 4 digits
    for (int32_t i = 7; i >= 4; --i) {
        uint32_t hexVal = (mData[1] >> (i * 4)) & 0xF;
        output[idx++] = HEX_TO_LITERAL[hexVal];
    }

    output[idx++] = '-';

    // Third group: 4 digits
    for (int32_t i = 3; i >= 0; --i) {
        uint32_t hexVal = (mData[1] >> (i * 4)) & 0xF;
        output[idx++] = HEX_TO_LITERAL[hexVal];
    }

    output[idx++] = '-';

    // Fourth group: 4 digits
    for (int32_t i = 7; i >= 4; --i) {
        uint32_t hexVal = (mData[2] >> (i * 4)) & 0xF;
        output[idx++] = HEX_TO_LITERAL[hexVal];
    }

    output[idx++] = '-';

    // Fifth group: 12 digits
    for (int32_t i = 3; i >= 0; --i) {
        uint32_t hexVal = (mData[2] >> (i * 4)) & 0xF;
        output[idx++] = HEX_TO_LITERAL[hexVal];
    }

    for (int32_t i = 7; i >= 0; --i) {
        uint32_t hexVal = (mData[3] >> (i * 4)) & 0xF;
        output[idx++] = HEX_TO_LITERAL[hexVal];
    }

    return FString(output.getData(), output.length());
}

FUuid UUIDGenerator::GenerateRandom() {
    return FPlatform::GenerateUUID();
}