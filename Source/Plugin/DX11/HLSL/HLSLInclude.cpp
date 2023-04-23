#include "HLSLInclude.h"
#include "FileSystem/FileSystem.h"

HRESULT FHLSLInclude::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData,
                           UINT *pBytes) {
    // TODO: Update PATH
    FPath path = TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Shader\\") + FString(pFileName);
    if (!FFileSystem::Exists(path)) {
        return E_FAIL;
    }

    auto file = FFileSystem::OpenFile(path);
    auto size = file->size();
    auto buf = q_alloc<char>(size + 1);
    *pBytes = UINT(size);
    *pBytes = file->read(buf, size);
    buf[size] = '\0';

    *ppData = buf;

    return S_OK;
}

HRESULT FHLSLInclude::Close(LPCVOID pData) {
    q_free((char *) pData);
    return S_OK;
}
