#include "Radian.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "Degree.h"

Radian::Radian(float v) : mValue(v) { }
Radian::Radian(const Radian &r) : mValue(r.mValue) { }
Radian::Radian(const Degree &d) : mValue(static_cast<float>(d) * (static_cast<float>(M_PI) / 180.f)) { }

Radian &Radian::operator=(const float &f) { mValue = f; return *this; }
Radian &Radian::operator=(const Radian &r) { mValue = r.mValue; return *this; }
Radian &Radian::operator=(const Degree &d) { mValue = static_cast<float>(d) * static_cast<float>(M_PI) / 180.0f; return *this; }
