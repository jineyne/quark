#pragma once

#include "Application.h"
#include "Material/Shader.h"
#include "Resource/ResourceHandle.h"
#include "Misc/Module.h"

QCLASS()
class DLL_EXPORT BuiltinResource {
private:
    static constexpr const TCHAR *ShaderFolder = TEXT("Shader");

    FResourceHandle<Shader> mForwardRenderingShader;

public:
    BuiltinResource();
    ~BuiltinResource();

public:
    const FResourceHandle<Shader> &getForwardRenderingShader();
};
