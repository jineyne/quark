#include "HLSLInclude.h"
#include "FileSystem/FileSystem.h"

HRESULT HLSLInclude::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData,
                          UINT *pBytes) {
    // TODO: Update PATH
    Path path = Path::Combine(String(RAW_APP_ROOT), TEXT("Asset\\Shader\\"));
    path.setFilename(String(pFileName));

    if (!FileSystem::Exists(path)) {
        return E_FAIL;
    }

    auto file = FileSystem::OpenFile(path);
    auto size = file->size();
    auto buf = q_alloc<char>(size + 1);
    *pBytes = UINT(size);
    *pBytes = file->read(buf, size);
    buf[size] = '\0';

    *ppData = buf;

    return S_OK;
}

HRESULT HLSLInclude::Close(LPCVOID pData) {
    q_free((char *) pData);
    return S_OK;
}
