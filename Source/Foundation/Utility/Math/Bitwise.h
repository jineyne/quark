#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

class DLL_EXPORT Bitwise {
public:
    static void IntWrite(void *dest, const int32_t n, const uint32_t value) {
        switch (n) {
            case 1:
                ((uint8_t *) dest)[0] = (uint8_t) value;
                break;
            case 2:
                ((uint16_t *) dest)[0] = (uint16_t) value;
                break;
            case 3:
                ((uint8_t *) dest)[2] = (uint8_t) ((value >> 16) & 0xFF);
                ((uint8_t *) dest)[1] = (uint8_t) ((value >> 8) & 0xFF);
                ((uint8_t *) dest)[0] = (uint8_t) (value & 0xFF);
                break;
            case 4:
                ((uint32_t *) dest)[0] = (uint32_t) value;
                break;
        }
    }

    static uint32_t IntRead(const void *src, int32_t n) {
        switch (n) {
            case 1:
                return ((uint8_t *) src)[0];
            case 2:
                return ((uint16_t *) src)[0];
            case 3:
                return ((uint32_t) ((uint8_t *) src)[0]) |
                       ((uint32_t) ((uint8_t *) src)[1] << 8) |
                       ((uint32_t) ((uint8_t *) src)[2] << 16);
            case 4:
                return ((uint32_t *) src)[0];
        }
        return 0; // ?
    }

    static uint32_t FixedToFixed(uint32_t value, uint32_t n, uint32_t p) {
        if (n > p) {
            value >>= n - p;
        } else if (n < p) {
            if (value == 0)
                value = 0;
            else if (value == (static_cast<uint32_t>(1) << n) - 1)
                value = (1 << p) - 1;
            else value = value * (1 << p) / ((1 << n) - 1);
        }
        return value;
    }

    static float UIntToUNorm(uint32_t value, uint32_t bits) {
        return (float) value / (float) ((1 << bits) - 1);
    }

    static float UIntToSNorm(uint32_t value, uint32_t bits) {
        return UIntToUNorm(value) * 2.0f - 1.0f;
    }

    template<uint32_t bits = 8>
    static float UIntToUNorm(uint32_t value) {
        return (float) value / (float) ((1 << bits) - 1);
    }

    static uint32_t UNormToUInt(float value, uint32_t bits) {
        if (value <= 0.0f) return 0;
        if (value >= 1.0f) return (1 << bits) - 1;
        return Math::RoundToInt(value * (1 << bits));
    }

    static uint32_t SNormToUInt(float value, uint32_t bits) {
        return UNormToUInt((value + 1.0f) * 0.5f, bits);
    }

    static float HalfToFloat(uint16_t y) {
        union {
            float f;
            uint32_t i;
        } v;
        v.i = HalfToFloatI(y);
        return v.f;
    }

    static uint32_t HalfToFloatI(uint16_t y) {
        int32_t s = (y >> 15) & 0x00000001;
        int32_t e = (y >> 10) & 0x0000001f;
        int32_t m = y & 0x000003ff;

        if (e == 0) {
            if (m == 0) // Plus or minus zero
            {
                return s << 31;
            } else // Denormalized number -- renormalize it
            {
                while (!(m & 0x00000400)) {
                    m <<= 1;
                    e -= 1;
                }

                e += 1;
                m &= ~0x00000400;
            }
        } else if (e == 31) {
            if (m == 0) // Inf
            {
                return (s << 31) | 0x7f800000;
            } else // NaN
            {
                return (s << 31) | 0x7f800000 | (m << 13);
            }
        }

        e = e + (127 - 15);
        m = m << 13;

        return (s << 31) | (e << 23) | m;
    }

    static uint16_t FloatToHalf(float i) {
        union {
            float f;
            uint32_t i;
        } v;
        v.f = i;
        return FloatToHalfI(v.i);
    }

    static uint16_t FloatToHalfI(uint32_t i) {
        int32_t s = (i >> 16) & 0x00008000;
        int32_t e = ((i >> 23) & 0x000000ff) - (127 - 15);
        int32_t m = i & 0x007fffff;

        if (e <= 0) {
            if (e < -10) {
                return 0;
            }
            m = (m | 0x00800000) >> (1 - e);

            return static_cast<uint16_t>(s | (m >> 13));
        } else if (e == 0xff - (127 - 15)) {
            if (m == 0) // Inf
            {
                return static_cast<uint16_t>(s | 0x7c00);
            } else    // NAN
            {
                m >>= 13;
                return static_cast<uint16_t>(s | 0x7c00 | m | (m == 0));
            }
        } else {
            if (e > 30) // Overflow
            {
                return static_cast<uint16_t>(s | 0x7c00);
            }

            return static_cast<uint16_t>(s | (e << 10) | (m >> 13));
        }
    }
};
