#pragma once

#include "CorePrerequisites.h"

#include "MaterialParams.h"

#include "RenderAPI/GpuParam.h"
#include "RenderAPI/GpuParams.h"

class FMaterial;

template<int DataType>
class DLL_EXPORT TMaterialDataCommon {
protected:
    using MaterialType = FMaterial;
    using MaterialParamsType = FMaterialParams;

protected:
    uint32_t mParamIndex;

    uint32_t mArraySize;

    MaterialType *mMaterial;

public:
    TMaterialDataCommon() = default;
    TMaterialDataCommon(const FString &name, MaterialType *material);

public:
    bool operator==(const std::nullptr_t &nullval) {
        return mMaterial == nullptr;
    }
};

template<class T>
class DLL_EXPORT TMaterialDataParam : public TMaterialDataCommon<TGpuDataParamInfo<T>::TypeId> {
private:
    using Base = TMaterialDataCommon<TGpuDataParamInfo<T>::TypeId>;

public:
    using TMaterialDataCommon<TGpuDataParamInfo<T>::TypeId>::TMaterialDataCommon;

    void set(const T &dst, uint32_t arrayIdx = 0) const;
    T get(uint32_t arrayIdx = 0) const;
};

class DLL_EXPORT MaterialParamStruct : public TMaterialDataCommon<static_cast<uint32_t>(EGpuParamDataType::Struct)> {
private:
    using Base = TMaterialDataCommon<static_cast<uint32_t>(EGpuParamDataType::Struct)>;

public:
    using TMaterialDataCommon<static_cast<uint32_t>(EGpuParamDataType::Struct)>::TMaterialDataCommon;

    void set(const void *src, uint32_t sizeBytes, uint32_t arrayIdx = 0) const;
    void get(void *dst, uint32_t sizeBytes, uint32_t arrayIdx = 0) const;

    uint32_t getElementSize() const;
};

#define MATERIAL_PARAM_DECL_BEGIN(TYPE) \
class DLL_EXPORT MaterialParam##TYPE { \
    using MaterialType = FMaterial; using MaterialParamsType = FMaterialParams; using ThisType = F##TYPE *;\
    protected: uint32_t mParamIndex; \
    protected: MaterialType *mMaterial; \
    public: MaterialParam##TYPE() = default; \
    public: MaterialParam##TYPE(const FString &name, MaterialType *material); \
    public: bool operator==(const std::nullptr_t &nullval) { return mMaterial == nullptr; }

#define MATERIAL_PARAM_DECL_BEGIN_RESOURCE(TYPE) \
class DLL_EXPORT MaterialParam##TYPE { \
    using MaterialType = FMaterial; using MaterialParamsType = FMaterialParams; using ThisType = FResourceHandle<F##TYPE>;\
    protected: uint32_t mParamIndex; \
    protected: MaterialType *mMaterial; \
    public: MaterialParam##TYPE() = default; \
    public: MaterialParam##TYPE(const FString &name, MaterialType *material); \
    public: bool operator==(const std::nullptr_t &nullval) { return mMaterial == nullptr; }

#define MATERIAL_PARAM_DECL_BEGIN_EX(TYPE, TYPE_CLASS) \
class DLL_EXPORT MaterialParam##TYPE { \
    using MaterialType = FMaterial; using MaterialParamsType = FMaterialParams; using ThisType = TYPE_CLASS *;\
    protected: uint32_t mParamIndex; \
    protected: MaterialType *mMaterial; \
    public: MaterialParam##TYPE() = default; \
    public: MaterialParam##TYPE(const FString &name, MaterialType *material); \
    public: bool operator==(const std::nullptr_t &nullval) { return mMaterial == nullptr; }

#define MATERIAL_PARAM_DECL_DEFAULT_GETTERSETTER() \
    public: void set(ThisType src) const; public: ThisType get() const;

#define MATERIAL_PARAM_DECL_SETTER(...) public: void set(ThisType src, __VA_ARGS__) const;
#define MATERIAL_PARAM_DECL_GETTER() public: ThisType get() const;

#define MATERIAL_PARAM_DECL_END(TYPE) \
};

MATERIAL_PARAM_DECL_BEGIN_RESOURCE(Texture)
MATERIAL_PARAM_DECL_SETTER(const FTextureSurface &surface = FTextureSurface::Complete)
MATERIAL_PARAM_DECL_GETTER()
MATERIAL_PARAM_DECL_END(Texture)

MATERIAL_PARAM_DECL_BEGIN_EX(Buffer, FGpuBuffer)
MATERIAL_PARAM_DECL_DEFAULT_GETTERSETTER()
MATERIAL_PARAM_DECL_END(Buffer)

MATERIAL_PARAM_DECL_BEGIN(SamplerState)
MATERIAL_PARAM_DECL_DEFAULT_GETTERSETTER()
MATERIAL_PARAM_DECL_END(SamplerState)