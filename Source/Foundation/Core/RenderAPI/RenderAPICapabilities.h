#pragma once

#include "CorePrerequisites.h"

struct DLL_EXPORT Conventions {
    enum class EAxis : uint8_t {
        Up, Down
    };

    enum class EMatrixOrder : uint8_t  {
        ColumnMajor, RowMajor
    };

    EAxis uvYAxis = EAxis::Down;
    EAxis ndcYAxis = EAxis::Up;
    EMatrixOrder matrixOrder = EMatrixOrder::RowMajor;
};

class DLL_EXPORT RenderAPICapabilities final {
public:
    float minDepth = 0.0f;
    float maxDepth = 1.0f;
    Conventions conventions;
};