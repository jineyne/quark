#include "GpuProgramManager.h"

FGpuProgram *FGpuProgramManager::create(const FGpuProgramDesc &desc) {
    auto program = createInternal(desc);
    program->initialize();

    return program;
}

FGpuProgramBytecode *FGpuProgramManager::compileBytecode(const FGpuProgramDesc &desc) {
    auto bytecode = new FGpuProgramBytecode();
    bytecode->compilerId = TEXT("Null");

    return bytecode;
}
