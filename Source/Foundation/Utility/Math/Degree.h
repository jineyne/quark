#pragma once

#include "UtilityCore.h"

struct FRadian;

struct DLL_EXPORT FDegree {
private:
    float mValue;

public:
    FDegree(float v = 0.0f);
    FDegree(const FDegree& d);
    FDegree(const FRadian& r);

    operator float() const { return mValue; }

    FDegree& operator = (const float& f);
    FDegree& operator = (const FDegree& r);
    FDegree& operator = (const FRadian& d);

public:
    bool operator == (const FDegree& r) const { return mValue == r.mValue; }
    bool operator != (const FDegree& r) const { return mValue != r.mValue; }
    bool operator <  (const FDegree& r) const { return mValue < r.mValue; }
    bool operator <= (const FDegree& r) const { return mValue <= r.mValue; }
    bool operator >  (const FDegree& r) const { return mValue > r.mValue; }
    bool operator >= (const FDegree& r) const { return mValue >= r.mValue; }

    FDegree operator + (const float& f) const { return FDegree(mValue + f); }
    FDegree operator + (const FDegree& r) const { return FDegree(mValue + r.mValue); }
    FDegree operator - () const { return FDegree(-mValue); }
    FDegree operator - (const float& f) const { return FDegree(mValue - f); }
    FDegree operator - (const FDegree& r) const { return FDegree(mValue - r.mValue); }
    FDegree operator * (const float& f) const { return FDegree(mValue * f); }
    FDegree operator * (const FDegree& r) const { return FDegree(mValue * r.mValue); }
    FDegree operator / (const float& f) const { return FDegree(mValue / f); }
    FDegree operator / (const FDegree& r) const { return FDegree(mValue / r.mValue); }

    FDegree& operator += (const float& f) { mValue += f; return *this; }
    FDegree& operator += (const FDegree& r) { mValue += r.mValue; return *this; }
    FDegree& operator -= (const float& f) { mValue -= f; return *this; }
    FDegree& operator -= (const FDegree& r) { mValue -= r.mValue; return *this; }
    FDegree& operator *= (const float& f) { mValue *= f; return *this; }
    FDegree& operator *= (const FDegree& r) { mValue *= r.mValue; return *this; }
    FDegree& operator /= (const float& f) { mValue /= f; return *this; }
    FDegree& operator /= (const FDegree& r) { mValue /= r.mValue; return *this; }
};
