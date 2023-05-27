#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

struct DLL_EXPORT DataBlob {
    uint8_t *data = nullptr;
    uint32_t size = 0;
};