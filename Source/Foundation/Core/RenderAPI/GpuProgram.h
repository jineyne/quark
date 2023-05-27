#pragma once

#include "CorePrerequisites.h"
#include "GpuParamDesc.h"
#include "VertexDeclaration.h"
#include "Misc/DataBlob.h"

enum class EGpuProgramType {
    Vertex,
    Fragment,
    Geometry,
    Hull,
    Domain,
    Compute,
    Count,
};

struct GpuProgramDesc {
    String source;
    String entryPoint;
    String language;

    EGpuProgramType type = EGpuProgramType::Vertex;
    char *bytecode = nullptr;
};

struct DLL_EXPORT GpuProgramBytecode {
    DataBlob instructions;
    GpuParamDesc *paramDesc;
    TArray<VertexElement> vertexInput;
    String messages;
    String compilerId;
    uint32_t compilerVersion = 0;
};

class DLL_EXPORT GpuProgram {
public:
    bool mIsCompiled = false;
    String mCompileMessage = String::Empty;

    GpuParamDesc *mParametersDesc = nullptr;

    EGpuProgramType mType;
    String mEntryPoint = String::Empty;

    String mSource = String::Empty;

    VertexDeclaration *mInputDeclaration = nullptr;
    GpuProgramBytecode *mBytecode = nullptr;

public:
    virtual ~GpuProgram();

public:
    static GpuProgram *New(const GpuProgramDesc &desc);
    static GpuProgramBytecode *CompileBytecode(const GpuProgramDesc& desc);

public:
    virtual void initialize();

    virtual bool isSupported() const;
    virtual bool isCompiled() const { return mIsCompiled; }

    String getCompileErrorMessage() const { return mCompileMessage; }
    GpuParamDesc *getParamDesc() const { return mParametersDesc; }
    EGpuProgramType getType() const { return mType; }
    VertexDeclaration *getInputDeclaration() const { return mInputDeclaration; }

protected:
    GpuProgram(const GpuProgramDesc &desc);
};
