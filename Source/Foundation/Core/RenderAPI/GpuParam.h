#pragma once

#include "CorePrerequisites.h"
#include "GpuParamDesc.h"
#include "Math/Matrix4.h"
#include "Image/Texture.h"
#include "Image/TextureSurface.h"

template<class Type>
struct TTransposePolicy {
    static Type Transpose(const Type& value) { return value; }
    static bool TransposeEnabled(bool enabled) { return false; }
};

/**	Transpose policy for 4x4 matrix. */
template<>
struct TTransposePolicy<Matrix4> {
    static Matrix4 Transpose(const Matrix4& value) { return Matrix4::Transpose(value); }
    static bool TransposeEnabled(bool enabled) { return enabled; }
};

template <class T>
class DLL_EXPORT GpuDataParam {
private:
    using GpuParamBlockType = GpuParamBlockBuffer;
    using GpuParamsType = GpuParams;

protected:
    GpuParamsType *mParent;
    GpuParamDataDesc *mParamDesc;

public:
    GpuDataParam();
    GpuDataParam(GpuParamDataDesc *paramDesc, GpuParamsType *parent);

public:
    void set(const T &value, uint32_t arrayIdx = 0) const;
    T get(uint32_t arrayIdx = 0);

    const GpuParamDataDesc &getDesc() const { return *mParamDesc; }

    bool operator==(const std::nullptr_t &nullval) const {
        return mParamDesc == nullptr;
    }
};

class DLL_EXPORT GpuParamBuffer {
private:
    using GpuParamsType = GpuParams ;
    using BufferType = GpuBuffer ;

protected:
    GpuParamsType *mParent;

    GpuParamObjectDesc *mParamDesc;

public:
    GpuParamBuffer();
    GpuParamBuffer(GpuParamObjectDesc *paramDesc, GpuParamsType *parent);

public:
    void set(BufferType *buffer) const;
    BufferType *get() const;

    const GpuParamObjectDesc &getDesc() const { return *mParamDesc; }

    bool operator==(const std::nullptr_t &nullval) const {
        return mParamDesc == nullptr;
    }
};

class DLL_EXPORT GpuParamTexture {
private:
    using GpuParamsType = GpuParams;
    using TextureType = FResourceHandle<Texture>;

protected:
    GpuParamsType *mParent;

    GpuParamObjectDesc *mParamDesc;

public:
    GpuParamTexture();
    GpuParamTexture(GpuParamObjectDesc *paramDesc, GpuParamsType *parent);

public:
    void set(const TextureType &texture, const TextureSurface &surface = TextureSurface::Complete) const;
    TextureType get() const;

    const GpuParamObjectDesc &getDesc() const { return *mParamDesc; }

    bool operator==(const std::nullptr_t &nullval) const {
        return mParamDesc == nullptr;
    }

private:
    friend class FGpuParams;
};

class DLL_EXPORT GpuParamSamplerState {
private:
    using GpuParamsType = GpuParams;
    using SamplerType = SamplerState;

protected:
    GpuParamsType *mParent;

    GpuParamObjectDesc *mParamDesc;

public:
    GpuParamSamplerState();
    GpuParamSamplerState(GpuParamObjectDesc *paramDesc, GpuParamsType *parent);

public:
    void set(SamplerType *samplerState) const;
    SamplerType *get() const;

    const GpuParamObjectDesc &getDesc() const { return *mParamDesc; }

    bool operator==(const std::nullptr_t &nullval) const {
        return mParamDesc == nullptr;
    }

private:
    friend class GpuParams;
};