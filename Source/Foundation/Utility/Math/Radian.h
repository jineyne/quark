#pragma once

#include "UtilityCore.h"
#include "Radian.g.h"

struct Degree;

QSTRUCT()
struct DLL_EXPORT Radian {
    GENERATED_BODY()

private:
    QPROPERTY()
    float mValue;

public:
    Radian(float v = 0.0f);
    Radian(const Radian& r);
    Radian(const Degree& d);

public:
    operator float() const { return mValue; }

    Radian& operator = (const float& f);
    Radian& operator = (const Radian& r);
    Radian& operator = (const Degree& d);

    bool operator == (const Radian& r) const { return mValue == r.mValue; }
    bool operator != (const Radian& r) const { return mValue != r.mValue; }
    bool operator <  (const Radian& r) const { return mValue < r.mValue; }
    bool operator <= (const Radian& r) const { return mValue <= r.mValue; }
    bool operator >  (const Radian& r) const { return mValue > r.mValue; }
    bool operator >= (const Radian& r) const { return mValue >= r.mValue; }

    Radian operator + (const float& f) const { return Radian(mValue + f); }
    Radian operator + (const Radian& r) const { return Radian(mValue + r.mValue); }
    Radian operator - () const { return Radian(-mValue); }
    Radian operator - (const float& f) const { return Radian(mValue - f); }
    Radian operator - (const Radian& r) const { return Radian(mValue - r.mValue); }
    Radian operator * (const float& f) const { return Radian(mValue * f); }
    Radian operator * (const Radian& r) const { return Radian(mValue * r.mValue); }
    Radian operator / (const float& f) const { return Radian(mValue / f); }
    Radian operator / (const Radian& r) const { return Radian(mValue / r.mValue); }

    Radian& operator += (const float& f) { mValue += f; return *this; }
    Radian& operator += (const Radian& r) { mValue += r.mValue; return *this; }
    Radian& operator -= (const float& f) { mValue -= f; return *this; }
    Radian& operator -= (const Radian& r) { mValue -= r.mValue; return *this; }
    Radian& operator *= (const float& f) { mValue *= f; return *this; }
    Radian& operator *= (const Radian& r) { mValue *= r.mValue; return *this; }
    Radian& operator /= (const float& f) { mValue /= f; return *this; }
    Radian& operator /= (const Radian& r) { mValue /= r.mValue; return *this; }
};
