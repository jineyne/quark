#pragma once

#include "DX11PRerequisites.h"
#include "Manager/GpuProgramManager.h"
#include "DX11GpuProgramManager.g.h"

QCLASS()
class DX11GpuProgramManager : public GpuProgramManager {
    GENERATED_BODY()

public:
    GpuProgramBytecode *compileBytecode(const GpuProgramDesc& desc) override;

protected:
    GpuProgram *createInternal(const GpuProgramDesc &desc) override;
};