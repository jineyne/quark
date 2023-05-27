#include "GpuProgramManager.h"

GpuProgram *GpuProgramManager::create(const GpuProgramDesc &desc) {
    auto program = createInternal(desc);
    program->initialize();

    return program;
}

GpuProgramBytecode *GpuProgramManager::compileBytecode(const GpuProgramDesc &desc) {
    auto bytecode = q_new<GpuProgramBytecode>();
    bytecode->compilerId = TEXT("Null");

    return bytecode;
}
