#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/GpuProgram.h"
#include "Misc/Module.h"
#include "GpuProgramManager.g.h"

QCLASS(abstract)
class DLL_EXPORT FGpuProgramManager : public TModule<FGpuProgramManager> {
    GENERATED_BODY()

public:
    virtual ~FGpuProgramManager() = default;

public:
    FGpuProgram *create(const FGpuProgramDesc &desc);
    virtual FGpuProgramBytecode *compileBytecode(const FGpuProgramDesc& desc);

protected:
    virtual FGpuProgram *createInternal(const FGpuProgramDesc &desc) = 0;

private:
    friend class GpuProgram;
};