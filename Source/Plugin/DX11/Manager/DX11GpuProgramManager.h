#pragma once

#include "DX11PRerequisites.h"
#include "Manager/GpuProgramManager.h"
#include "DX11GpuProgramManager.g.h"

QCLASS()
class FDX11GpuProgramManager : public FGpuProgramManager {
    GENERATED_BODY()

public:
    FGpuProgramBytecode *compileBytecode(const FGpuProgramDesc& desc) override;

protected:
    FGpuProgram *createInternal(const FGpuProgramDesc &desc) override;
};