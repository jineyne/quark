#pragma once

#include "Application.h"
#include "Material/Shader.h"
#include "Resource/ResourceHandle.h"
#include "Misc/Module.h"

QCLASS()
class DLL_EXPORT FBuiltinResource {
private:
    static constexpr const TCHAR *ShaderFolder = TEXT("Shader");

    FResourceHandle<FShader> mForwardRenderingShader;

public:
    FBuiltinResource();
    ~FBuiltinResource();

public:
    const FResourceHandle<FShader> &getForwardRenderingShader();
};
