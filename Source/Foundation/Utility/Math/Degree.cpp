#include "Degree.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "Radian.h"

Degree::Degree(float v) : mValue(v) { }
Degree::Degree(const Degree &d) : mValue(d.mValue) { }
Degree::Degree(const Radian &r) : mValue(static_cast<float>(r) * 180.0f / static_cast<float>(M_PI)) { }

Degree &Degree::operator=(const float &f) { mValue = f; return *this; }
Degree &Degree::operator=(const Degree &d) { mValue = d.mValue; return *this; }
Degree &Degree::operator=(const Radian &r) { mValue = static_cast<float>(r) * 180.0f / static_cast<float>(M_PI); return *this; }
