#include "GpuProgram.h"
#include "Manager/GpuProgramManager.h"

FGpuProgram::FGpuProgram(const FGpuProgramDesc &desc)
        : mType(desc.type), mEntryPoint(desc.entryPoint), mSource(desc.source) {
    mParametersDesc = new FGpuParamDesc();
}

FGpuProgram::~FGpuProgram() {
    delete mInputDeclaration;
    delete mParametersDesc;
    delete mBytecode;
}

FGpuProgram *FGpuProgram::New(const FGpuProgramDesc &desc) {
    return FGpuProgramManager::Instance().create(desc);
}

FGpuProgramBytecode *FGpuProgram::CompileBytecode(const FGpuProgramDesc &desc) {
    return FGpuProgramManager::Instance().compileBytecode(desc);
}

void FGpuProgram::initialize() {

}

bool FGpuProgram::isSupported() const {
    return true;
}

