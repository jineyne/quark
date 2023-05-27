#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/GpuProgram.h"
#include "Misc/Module.h"
#include "GpuProgramManager.g.h"

QCLASS(abstract)
class DLL_EXPORT GpuProgramManager : public TModule<GpuProgramManager> {
    GENERATED_BODY()

public:
    virtual ~GpuProgramManager() = default;

public:
    GpuProgram *create(const GpuProgramDesc &desc);
    virtual GpuProgramBytecode *compileBytecode(const GpuProgramDesc& desc);

protected:
    virtual GpuProgram *createInternal(const GpuProgramDesc &desc) = 0;

private:
    friend class GpuProgram;
};