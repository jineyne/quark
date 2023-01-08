#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

template <typename T>
void memzero(T &data) {
    memset(&data, 0, sizeof(T));
}

template <typename T>
void memzero(T &data, size_t size) {
    memset(&data, 0, size);
}
