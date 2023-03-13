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

struct FGpuProgramDesc {
    FString source;
    FString entryPoint;
    FString language;

    EGpuProgramType type = EGpuProgramType::Vertex;
    char *bytecode = nullptr;
};

struct DLL_EXPORT FGpuProgramBytecode {
    FDataBlob instructions;
    FGpuParamDesc *paramDesc;
    TArray<FVertexElement> vertexInput;
    FString messages;
    FString compilerId;
    uint32_t compilerVersion = 0;
};

class DLL_EXPORT FGpuProgram {
public:
    bool mIsCompiled = false;
    FString mCompileMessage = FString::Empty;

    FGpuParamDesc *mParametersDesc = nullptr;

    EGpuProgramType mType;
    FString mEntryPoint = FString::Empty;

    FString mSource = FString::Empty;

    FVertexDeclaration *mInputDeclaration = nullptr;
    FGpuProgramBytecode *mBytecode = nullptr;

public:
    virtual ~FGpuProgram();

public:
    static FGpuProgram *New(const FGpuProgramDesc &desc);
    static FGpuProgramBytecode *CompileBytecode(const FGpuProgramDesc& desc);

public:
    virtual void initialize();

    virtual bool isSupported() const;
    virtual bool isCompiled() const { return mIsCompiled; }

    FString getCompileErrorMessage() const { return mCompileMessage; }
    FGpuParamDesc *getParamDesc() const { return mParametersDesc; }
    EGpuProgramType getType() const { return mType; }
    FVertexDeclaration *getInputDeclaration() const { return mInputDeclaration; }

protected:
    FGpuProgram(const FGpuProgramDesc &desc);
};
