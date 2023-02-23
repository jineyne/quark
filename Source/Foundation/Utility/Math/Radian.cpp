#include "Radian.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "Degree.h"

FRadian::FRadian(float v) : mValue(v) { }
FRadian::FRadian(const FRadian &r) : mValue(r.mValue) { }
FRadian::FRadian(const FDegree &d) : mValue(static_cast<float>(d) * (static_cast<float>(M_PI) / 180.f)) { }

FRadian &FRadian::operator=(const float &f) { mValue = f; return *this; }
FRadian &FRadian::operator=(const FRadian &r) { mValue = r.mValue; return *this; }
FRadian &FRadian::operator=(const FDegree &d) { mValue = static_cast<float>(d) * static_cast<float>(M_PI) / 180.0f; return *this; }
