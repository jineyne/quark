#pragma once

#include "UtilityCore.h"
#include "Degree.g.h"

struct Radian;

QSTRUCT()
struct DLL_EXPORT Degree {
    GENERATED_BODY()
private:

    QPROPERTY()
    float mValue;

public:
    Degree(float v = 0.0f);
    Degree(const Degree& d);
    Degree(const Radian& r);

    operator float() const { return mValue; }

    Degree& operator = (const float& f);
    Degree& operator = (const Degree& r);
    Degree& operator = (const Radian& d);

public:
    bool operator == (const Degree& r) const { return mValue == r.mValue; }
    bool operator != (const Degree& r) const { return mValue != r.mValue; }
    bool operator <  (const Degree& r) const { return mValue < r.mValue; }
    bool operator <= (const Degree& r) const { return mValue <= r.mValue; }
    bool operator >  (const Degree& r) const { return mValue > r.mValue; }
    bool operator >= (const Degree& r) const { return mValue >= r.mValue; }

    Degree operator + (const float& f) const { return Degree(mValue + f); }
    Degree operator + (const Degree& r) const { return Degree(mValue + r.mValue); }
    Degree operator - () const { return Degree(-mValue); }
    Degree operator - (const float& f) const { return Degree(mValue - f); }
    Degree operator - (const Degree& r) const { return Degree(mValue - r.mValue); }
    Degree operator * (const float& f) const { return Degree(mValue * f); }
    Degree operator * (const Degree& r) const { return Degree(mValue * r.mValue); }
    Degree operator / (const float& f) const { return Degree(mValue / f); }
    Degree operator / (const Degree& r) const { return Degree(mValue / r.mValue); }

    Degree& operator += (const float& f) { mValue += f; return *this; }
    Degree& operator += (const Degree& r) { mValue += r.mValue; return *this; }
    Degree& operator -= (const float& f) { mValue -= f; return *this; }
    Degree& operator -= (const Degree& r) { mValue -= r.mValue; return *this; }
    Degree& operator *= (const float& f) { mValue *= f; return *this; }
    Degree& operator *= (const Degree& r) { mValue *= r.mValue; return *this; }
    Degree& operator /= (const float& f) { mValue /= f; return *this; }
    Degree& operator /= (const Degree& r) { mValue /= r.mValue; return *this; }
};
