#pragma once

#include "Prerequisites.h"
#include "AITypes.g.h"

QENUM()
enum class EAIStatus {
    Success     QENTRY(),
    Failure     QENTRY(),
    Running     QENTRY()
};