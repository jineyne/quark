#include "Degree.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "Radian.h"

FDegree::FDegree(float v) : mValue(v) { }
FDegree::FDegree(const FDegree &d) : mValue(d.mValue) { }
FDegree::FDegree(const FRadian &r) : mValue(static_cast<float>(r) * 180.0f / static_cast<float>(M_PI)) { }

FDegree &FDegree::operator=(const float &f) { mValue = f; return *this; }
FDegree &FDegree::operator=(const FDegree &d) { mValue = d.mValue; return *this; }
FDegree &FDegree::operator=(const FRadian &r) { mValue = static_cast<float>(r) * 180.0f / static_cast<float>(M_PI); return *this; }
