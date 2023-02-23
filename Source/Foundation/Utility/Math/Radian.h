#pragma once

#include "UtilityCore.h"

struct FDegree;

struct DLL_EXPORT FRadian {
private:
    float mValue;

public:
    FRadian(float v = 0.0f);
    FRadian(const FRadian& r);
    FRadian(const FDegree& d);

public:
    operator float() const { return mValue; }

    FRadian& operator = (const float& f);
    FRadian& operator = (const FRadian& r);
    FRadian& operator = (const FDegree& d);

    bool operator == (const FRadian& r) const { return mValue == r.mValue; }
    bool operator != (const FRadian& r) const { return mValue != r.mValue; }
    bool operator <  (const FRadian& r) const { return mValue < r.mValue; }
    bool operator <= (const FRadian& r) const { return mValue <= r.mValue; }
    bool operator >  (const FRadian& r) const { return mValue > r.mValue; }
    bool operator >= (const FRadian& r) const { return mValue >= r.mValue; }

    FRadian operator + (const float& f) const { return FRadian(mValue + f); }
    FRadian operator + (const FRadian& r) const { return FRadian(mValue + r.mValue); }
    FRadian operator - () const { return FRadian(-mValue); }
    FRadian operator - (const float& f) const { return FRadian(mValue - f); }
    FRadian operator - (const FRadian& r) const { return FRadian(mValue - r.mValue); }
    FRadian operator * (const float& f) const { return FRadian(mValue * f); }
    FRadian operator * (const FRadian& r) const { return FRadian(mValue * r.mValue); }
    FRadian operator / (const float& f) const { return FRadian(mValue / f); }
    FRadian operator / (const FRadian& r) const { return FRadian(mValue / r.mValue); }

    FRadian& operator += (const float& f) { mValue += f; return *this; }
    FRadian& operator += (const FRadian& r) { mValue += r.mValue; return *this; }
    FRadian& operator -= (const float& f) { mValue -= f; return *this; }
    FRadian& operator -= (const FRadian& r) { mValue -= r.mValue; return *this; }
    FRadian& operator *= (const float& f) { mValue *= f; return *this; }
    FRadian& operator *= (const FRadian& r) { mValue *= r.mValue; return *this; }
    FRadian& operator /= (const float& f) { mValue /= f; return *this; }
    FRadian& operator /= (const FRadian& r) { mValue /= r.mValue; return *this; }
};
