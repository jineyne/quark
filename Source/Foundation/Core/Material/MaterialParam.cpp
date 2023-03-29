#include "MaterialParam.h"

#include "Material/Material.h"
#include "Material/MaterialParams.h"

template<int DataType>
TMaterialDataCommon<DataType>::TMaterialDataCommon(const FString &name, MaterialType *material)
        : mParamIndex(0), mArraySize(0), mMaterial(nullptr) {
    if (material != nullptr) {
        auto params = material->getParams();

        uint32_t paramIndex;
        auto result = params->getParamIndex(name, FMaterialParams::EParamType::Data,
                                            static_cast<EGpuParamDataType>(DataType), 0, paramIndex);

        if (result == FMaterialParams::EGetParamResult::Success) {
            const auto data = params->getParamData(paramIndex);
            this->mMaterial = material;
            this->mParamIndex = paramIndex;
            this->mArraySize = data->arraySize;
        } else {
            params->reportGetParamError(result, name, 0);
        }
    }
}

template<class T>
void TMaterialDataParam<T>::set(const T &value, uint32_t arrayIdx) const {
    if (this->mMaterial == nullptr) {
        return;
    }

    if (arrayIdx >= mArraySize) {
        LOG(FLogMaterial, Warning, TEXT("Array index out of range. Provided index was %ld but array length is %ld"), arrayIdx, this->mArraySize);
        return;
    }

    auto params = this->mMaterial->getParams();
    const auto data = params->getParamData(this->mParamIndex);

    params->setDataParam(*data, arrayIdx, value);
    this->mMaterial->markCoreDirty();

}

template<class T>
T TMaterialDataParam<T>::get(uint32_t arrayIdx) const {
    T output{};
    if (this->mMaterial == nullptr || arrayIdx >= this->mArraySize) {
        return output;
    }

    auto params = this->mMaterial->getParams();
    const auto data = params->getParamData(this->mParamIndex);

    params->getDataParam(*data, arrayIdx, output);
    return output;
}

#define MATERIAL_DATA_PARAM_INSTATIATE(type)										\
	template class TMaterialDataCommon<TGpuDataParamInfo<type>::TypeId>;		    \
	template class TMaterialDataParam<type>;

MATERIAL_DATA_PARAM_INSTATIATE(float);
MATERIAL_DATA_PARAM_INSTATIATE(FVector2);
MATERIAL_DATA_PARAM_INSTATIATE(FVector3);
MATERIAL_DATA_PARAM_INSTATIATE(int);
MATERIAL_DATA_PARAM_INSTATIATE(FMatrix4);
MATERIAL_DATA_PARAM_INSTATIATE(FColor);

template class TMaterialDataCommon<static_cast<uint32_t>(EGpuParamDataType::Struct)>;


#undef MATERIAL_DATA_PARAM_INSTATIATE

void MaterialParamStruct::set(const void *src, uint32_t sizeBytes, uint32_t arrayIdx) const {
    if (mMaterial == nullptr) {
        return;
    }

    if (arrayIdx >= mArraySize) {
        LOG(FLogMaterial, Warning, TEXT("Array index out of range. Provided index was {0} but array length is {1}"),
            arrayIdx, this->mArraySize);
        return;
    }

    typename Base::MaterialParamsType *params = mMaterial->getParams();
    const auto data = params->getParamData(mParamIndex);

    params->setStructData(*data, src, sizeBytes, arrayIdx);
    mMaterial->markCoreDirty();
}

void MaterialParamStruct::get(void *dst, uint32_t sizeBytes, uint32_t arrayIdx) const {
    if (mMaterial == nullptr || arrayIdx >= mArraySize) {
        return;
    }

    auto params = mMaterial->getParams();
    const auto data = params->getParamData(mParamIndex);

    params->getStructData(*data, dst, sizeBytes, arrayIdx);
}

uint32_t MaterialParamStruct::getElementSize() const {
    if (mMaterial == nullptr) {
        return 0;
    }

    auto params = mMaterial->getParams();
    const auto data = params->getParamData(mParamIndex);

    return params->getStructSize(*data);
}

#define MATERIAL_PARAM_IMPL_CONSTRUCTOR(TYPE) \
MaterialParam##TYPE::MaterialParam##TYPE(const FString &name, MaterialType *material) : mParamIndex(0), mMaterial(nullptr)

#define MATERIAL_PARAM_DEFAULT_CONSTRUCTOR(TYPE, PARAM_TYPE) \
MATERIAL_PARAM_IMPL_CONSTRUCTOR(TYPE) { \
    if (material != nullptr) { \
        auto params = material->getParams();\
        uint32_t idx;\
        auto result = params->getParamIndex(name, MaterialParams::ParamType::PARAM_TYPE, GpuParamDataType::Unknown, 0, idx);\
        if(result == MaterialParams::GetParamResult::Success) { mMaterial = material; mParamIndex = idx; } \
        else { params->reportGetParamError(result, name, 0); }\
    }\
}

#define MATERIAL_PARAM_IMPL_SETTER(TYPE) void MaterialParam##TYPE::set(ThisType src) const

#define MATERIAL_PARAM_IMPL_SETTER_EX(TYPE, ...) \
void MaterialParam##TYPE::set(ThisType src, __VA_ARGS__) const

#define MATERIAL_PARAM_IMPL_DEFAULT_SETTER(TYPE) \
    MATERIAL_PARAM_IMPL_SETTER(TYPE) {\
        if (mMaterial == nullptr) { return; }\
        auto params = mMaterial->getParams(); \
        const auto data = params->getParamData(mParamIndex);\
        params->set##TYPE(*data, src);\
        mMaterial->markCoreDirty();\
        mMaterial->markDependenciesDirty();\
    }

#define MATERIAL_PARAM_IMPL_GETTER(TYPE) \
MaterialParam##TYPE::ThisType MaterialParam##TYPE::get() const

#define MATERIAL_PARAM_IMPL_DEFAULT_GETTER(TYPE) \
    MATERIAL_PARAM_IMPL_GETTER(TYPE) {\
        ThisType dst = nullptr; \
        if (mMaterial == nullptr) { return dst; }\
        auto params = mMaterial->getParams();\
        const auto data = params->getParamData(mParamIndex); \
        params->get##TYPE(*data, dst); \
        return dst; \
    }

MATERIAL_PARAM_IMPL_CONSTRUCTOR(Texture) {
    if (material != nullptr) {
        auto params = material->getParams();
        uint32_t idx;
        auto result = params->getParamIndex(name, FMaterialParams::EParamType::Texture, EGpuParamDataType::Unknown, 0, idx);
        if (result == FMaterialParams::EGetParamResult::Success) {
            mMaterial = material; mParamIndex = idx;
        } else {
            params->reportGetParamError(result, name, 0);
        }
    }
}

MATERIAL_PARAM_IMPL_SETTER_EX(Texture, const FTextureSurface &surface) {
    if (mMaterial == nullptr)
        return;

    auto params = mMaterial->getParams();
    const auto data = params->getParamData(mParamIndex);

    ThisType newValue = src;
    if (newValue == nullptr) {
        params->getDefaultTexture(*data, newValue);
    }

    params->setTexture(*data, newValue, surface);
    mMaterial->markCoreDirty();
    mMaterial->markDependenciesDirty();
    mMaterial->markResourcesDirty();
}

MATERIAL_PARAM_IMPL_GETTER(Texture) {
    ThisType texture;
    if (mMaterial == nullptr)
        return texture;

    FTextureSurface surface;

    auto params = mMaterial->getParams();
    const auto data = params->getParamData(mParamIndex);

    params->getTexture(*data, texture, surface);
    return texture;
}

MATERIAL_PARAM_IMPL_CONSTRUCTOR(Buffer) {
    if (material != nullptr) {
        auto params = material->getParams();
        uint32_t idx;
        auto result = params->getParamIndex(name, FMaterialParams::EParamType::Buffer, EGpuParamDataType::Unknown, 0, idx);
        if (result == FMaterialParams::EGetParamResult::Success) {
            mMaterial = material; mParamIndex = idx;
        } else {
            params->reportGetParamError(result, name, 0);
        }
    }
}

MATERIAL_PARAM_IMPL_DEFAULT_SETTER(Buffer);

MATERIAL_PARAM_IMPL_DEFAULT_GETTER(Buffer);


MATERIAL_PARAM_IMPL_CONSTRUCTOR(SamplerState) {
    if (material != nullptr) {
        auto params = material->getParams();
        uint32_t idx;
        auto result = params->getParamIndex(name, FMaterialParams::EParamType::Sampler, EGpuParamDataType::Unknown, 0, idx);
        if (result == FMaterialParams::EGetParamResult::Success) {
            mMaterial = material; mParamIndex = idx;
        } else {
            params->reportGetParamError(result, name, 0);
        }
    }
}

MATERIAL_PARAM_IMPL_DEFAULT_SETTER(SamplerState);

MATERIAL_PARAM_IMPL_DEFAULT_GETTER(SamplerState);