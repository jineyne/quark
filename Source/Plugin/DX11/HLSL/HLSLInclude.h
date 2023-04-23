#pragma once

#include "DX11Prerequisites.h"

class DX11_EXPORT FHLSLInclude : public ID3DInclude {
public:
    HRESULT Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes) override;
    HRESULT Close(LPCVOID pData) override;
};