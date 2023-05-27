#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

class DLL_EXPORT Math {
public:
    static constexpr float BIGGEST_FLOAT_SMALLER_THAN_ONE = 0.99999994f;
    static double PI;

    template<typename T>
    static T Cos(T val) {
        if (val < 0) {
            val = -val;
        }
        if (val > 360) {
            val -= (T)(val / (T)360) * 360;
        }
        val *= PI / 180.0;
        double res = 0;
        double term = 1;
        int k = 0;
        while (res + term != res) {
            res += term;
            k += 2;
            term *= -val * val / k / (k - 1);
        }
        return res;
    }

    template<typename T>
    static T Sin(T val) {
        double sign = 1;
        if (val < 0) {
            sign = -1.0;
            val = -val;
        }
        if (val > 360) {
            val -= (T)(val / (T)360) * 360;
        }
        val *= PI / 180.0;
        double res = 0;
        double term = val;
        int k = 1;
        while (res + term != res) {
            res += term;
            k += 2;
            term *= -val * val / k / (k - 1);
        }

        return sign * res;
    }

    template<typename T>
    static int FloorToInt(T val) {
        assert(val >= std::numeric_limits<T>::min() && val <= std::numeric_limits<T>::max());

        // Negative values need offset in order to truncate towards negative infinity (cast truncates towards zero)
        return val >= 0.0f ? (int32_t)val : (int32_t)(val - BIGGEST_FLOAT_SMALLER_THAN_ONE);
    }

    template<typename T>
    static uint32_t FloorToPosInt(T val) {
        assert(val >= 0 && val <= std::numeric_limits<uint32_t>::max());

        return static_cast<uint32_t>(val);
    }

    static int32_t RoundToInt(float val) {
        return FloorToInt(val + 0.5f);
    }

    template<class T>
    static constexpr T DivideAndRoundUp(T n, T d) {
        return (n + d - 1) / d;
    }

    template<typename T>
    static constexpr T Abs(T t) {
        return std::abs(t);
    }

    template<typename T>
    static T Floor(T val) {
        return static_cast<T>(std::floor(val));
    }

    template<typename T>
    static T Repeat(T val, float length) {
        return val - Floor(val / length) * length;
    }

    template<typename T>
    static T PingPong(T val, T length) {
        val = Repeat(val, length * 2.0f);
        return length - Abs(val - length);
    }

    template<typename T>
    static T InvertSafe(T val) {
        return val != 0 ? 1 / val : 0;
    }

    template <typename T>
    static T Lerp(float t, T min, T max) {
        return (1.0f - t) * min + t * max;
    }

    template <typename T>
    static T Clamp(T val, T minval, T maxval) {
        assert (minval <= maxval && "Invalid clamp range");
        return std::max(std::min(val, maxval), minval);
    }

    static bool ApproxEquals(float a, float b,
                             float tolerance = std::numeric_limits<float>::epsilon()) {
        return fabs(b - a) <= tolerance;
    }

    /** Compare two doubles, using tolerance for inaccuracies. */
    static bool ApproxEquals(double a, double b,
                             double tolerance = std::numeric_limits<double>::epsilon()) {
        return fabs(b - a) <= tolerance;
    }
};
