#pragma once

#include "CorePrerequisites.h"
#include "GpuParamDesc.h"
#include "Math/Matrix4.h"
#include "Image/Texture.h"
#include "Image/TextureSurface.h"

template<class Type>
struct FTransposePolicy {
    static Type Transpose(const Type& value) { return value; }
    static bool TransposeEnabled(bool enabled) { return false; }
};

/**	Transpose policy for 4x4 matrix. */
template<>
struct FTransposePolicy<FMatrix4> {
    static FMatrix4 Transpose(const FMatrix4& value) { return FMatrix4::Transpose(value); }
    static bool TransposeEnabled(bool enabled) { return enabled; }
};

template <class T>
class DLL_EXPORT FGpuDataParam {
private:
    using GpuParamBlockType = FGpuParamBlockBuffer;
    using GpuParamsType = FGpuParams;

protected:
    GpuParamsType *mParent;
    FGpuParamDataDesc *mParamDesc;

public:
    FGpuDataParam();
    FGpuDataParam(FGpuParamDataDesc *paramDesc, GpuParamsType *parent);

public:
    void set(const T &value, uint32_t arrayIdx = 0) const;
    T get(uint32_t arrayIdx = 0);

    const FGpuParamDataDesc &getDesc() const { return *mParamDesc; }

    bool operator==(const std::nullptr_t &nullval) const {
        return mParamDesc == nullptr;
    }
};

class DLL_EXPORT FGpuParamBuffer {
private:
    using GpuParamsType = FGpuParams ;
    using BufferType = FGpuBuffer ;

protected:
    GpuParamsType *mParent;

    FGpuParamObjectDesc *mParamDesc;

public:
    FGpuParamBuffer();
    FGpuParamBuffer(FGpuParamObjectDesc *paramDesc, GpuParamsType *parent);

public:
    void set(BufferType *buffer) const;
    BufferType *get() const;

    const FGpuParamObjectDesc &getDesc() const { return *mParamDesc; }

    bool operator==(const std::nullptr_t &nullval) const {
        return mParamDesc == nullptr;
    }
};

class DLL_EXPORT FGpuParamTexture {
private:
    using GpuParamsType = FGpuParams;
    using TextureType = FResourceHandle<FTexture>;

protected:
    GpuParamsType *mParent;

    FGpuParamObjectDesc *mParamDesc;

public:
    FGpuParamTexture();
    FGpuParamTexture(FGpuParamObjectDesc *paramDesc, GpuParamsType *parent);

public:
    void set(const TextureType &texture, const FTextureSurface &surface = FTextureSurface::Complete) const;
    TextureType get() const;

    const FGpuParamObjectDesc &getDesc() const { return *mParamDesc; }

    bool operator==(const std::nullptr_t &nullval) const {
        return mParamDesc == nullptr;
    }

private:
    friend class GpuParams;
};

class DLL_EXPORT FGpuParamSamplerState {
private:
    using GpuParamsType = FGpuParams;
    using SamplerType = FSamplerState;

protected:
    GpuParamsType *mParent;

    FGpuParamObjectDesc *mParamDesc;

public:
    FGpuParamSamplerState();
    FGpuParamSamplerState(FGpuParamObjectDesc *paramDesc, GpuParamsType *parent);

public:
    void set(SamplerType *samplerState) const;
    SamplerType *get() const;

    const FGpuParamObjectDesc &getDesc() const { return *mParamDesc; }

    bool operator==(const std::nullptr_t &nullval) const {
        return mParamDesc == nullptr;
    }

private:
    friend class GpuParams;
};