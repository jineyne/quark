#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

class DLL_EXPORT Base64 {
public:
    static size_t Encode(TCHAR *text, size_t length, TCHAR **encodedText);
    static size_t Decode(TCHAR *text, size_t length, TCHAR *decodedText);
};