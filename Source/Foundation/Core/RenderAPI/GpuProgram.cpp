#include "GpuProgram.h"
#include "Manager/GpuProgramManager.h"

GpuProgram::GpuProgram(const GpuProgramDesc &desc)
        : mType(desc.type), mEntryPoint(desc.entryPoint), mSource(desc.source) {
    mParametersDesc = q_new<GpuParamDesc>();
}

GpuProgram::~GpuProgram() {
    q_delete(mInputDeclaration);
    q_delete(mParametersDesc);
    q_delete(mBytecode);
}

GpuProgram *GpuProgram::New(const GpuProgramDesc &desc) {
    return GpuProgramManager::Instance().create(desc);
}

GpuProgramBytecode *GpuProgram::CompileBytecode(const GpuProgramDesc &desc) {
    return GpuProgramManager::Instance().compileBytecode(desc);
}

void GpuProgram::initialize() {

}

bool GpuProgram::isSupported() const {
    return true;
}

