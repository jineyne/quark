#pragma once

#include "UtilityCore.h"
#include "Misc.h"
#include "UUID.g.h"

QSTRUCT()
struct DLL_EXPORT FUuid {
    GENERATED_BODY()

public:
    static FUuid Empty;

private:
    QPROPERTY()
    uint32_t mData[4] = { 0, };

public:
    constexpr FUuid() = default;

    constexpr FUuid(uint32_t data1, uint32_t data2, uint32_t data3, uint32_t data4)
            : mData { data1, data2, data3, data4 } {}

    explicit FUuid(const FString &uuid);

public:
    constexpr bool operator==(const FUuid &rhs) const {
        return mData[0] == rhs.mData[0] && mData[1] == rhs.mData[1] && mData[2] == rhs.mData[2] && mData[3] == rhs.mData[3];
    }

    constexpr bool operator!=(const FUuid &rhs) const {
        return !(*this == rhs);
    }

    constexpr bool operator<(const FUuid &rhs) const {
        for(uint32_t i = 0; i < 4; i++) {
            if (mData[i] < rhs.mData[i]) {
                return true;
            } else if (mData[i] > rhs.mData[i]) {
                return false;
            }
        }

        return false;
    }

public:
    FString toString() const;

    constexpr bool empty() const {
        return mData[0] == 0 && mData[1] == 0 && mData[2] == 0 && mData[3] == 0;
    }

private:
    friend struct std::hash<FUuid>;
};

class DLL_EXPORT UUIDGenerator {
public:
    static FUuid GenerateRandom();
};

namespace std {
    template<>
    struct hash<FUuid> {
        size_t operator()(const FUuid &uuid) const {
            size_t hash = 0;

            CombineHash(hash, uuid.mData[0]);
            CombineHash(hash, uuid.mData[1]);
            CombineHash(hash, uuid.mData[2]);
            CombineHash(hash, uuid.mData[3]);

            return hash;
        }
    };
}