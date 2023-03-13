#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

class DLL_EXPORT FMath {
public:
    template<class T>
    static constexpr T DivideAndRoundUp(T n, T d) {
        return (n + d - 1) / d;
    }
};
