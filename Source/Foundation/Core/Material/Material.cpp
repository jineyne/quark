#include "Material.h"
#include "Exception/Exception.h"

DEFINE_LOG_CATEGORY(LogMaterial)

enum EMaterialLoadFlags {
    Load_None = 0,
    Load_Shader = 1,
    Load_All = 2
};

template<class T>
void copyParam(MaterialParams *from, Material *to, const String &name,
               const MaterialParams::ParamData &paramRef, uint32_t arraySize) {
    TMaterialDataParam<T> param;
    to->getParam(name, param);

    T paramData;
    for (uint32_t i = 0; i < arraySize; i++) {
        from->getDataParam(paramRef, i, paramData);
        param.set(paramData, i);
    }
}

Material::Material(Material::ShaderType *shader, const FShaderVariation &variation)
        : mShader(shader), mVariation(variation), mLoadFlags(Load_None)  {
    if (mLoadFlags != Load_All) {
        mLoadFlags = Load_All;

        auto oldParams = mParams;
        initializeTechniques();

        if (mTechniqueList.empty()) {
            return;
        }

        if (oldParams) {
            setParams(oldParams);
        }
    }
}

Material::~Material() {

}

Material *Material::New(Material::ShaderType *shader, const FShaderVariation &variation) {
    return q_new<Material>(shader, variation);
}

uint32_t Material::findTechnique(FindTechniqueDesc &desc) const {
    auto bestTechniqueIdx = static_cast<uint32_t>(-1);
    auto bestTechniqueScore = std::numeric_limits<uint32_t>::max();
    for (uint32_t i = 0; i < static_cast<uint32_t>(mTechniqueList.length()); i++) {
        bool foundMatch = true;
        for (uint32_t j = 0; j < desc.tagCount; j++) {
            if (!mTechniqueList[i]->hasTag(desc.tags[i])) {
                foundMatch = false;
                break;
            }
        }

        if (!foundMatch) {
            continue;
        }

        const auto &variation = mTechniqueList[i]->getVariation();
        const auto &varParams = variation.getParams();
        const auto &internalVarParams = variation.getParams();

        uint32_t matchedSearchParamsCount = 0;
        uint32_t matchedInternalParamsCount = 0;
        uint32_t currentScore = 0;

        for (auto &param : varParams) {
            enum class SearchResult {
                NoParam,
                NotMatching,
                Matching
            };

            auto matchesSearch = SearchResult::NoParam;
            if (desc.variation) {
                const auto &searchVarParams = desc.variation->getParams();
                const auto findSearch = searchVarParams.find(param.key);
                if (findSearch != nullptr) {
                    matchesSearch = (*findSearch).i == param.value.i ? SearchResult::Matching :
                                    SearchResult::NotMatching;
                }
            }

            auto matchesInternal = SearchResult::NoParam;
            const auto findInternal = internalVarParams.find(param.key);
            if (findInternal != nullptr) {
                matchesInternal = (*findInternal).i == param.value.i ? SearchResult::Matching :
                                  SearchResult::NotMatching;
            }

            switch (matchesSearch) {
                default:
                case SearchResult::NoParam:
                    switch (matchesInternal) {
                        default:
                        case SearchResult::NoParam:
                            currentScore += param.value.ui;
                            break;
                        case SearchResult::NotMatching:
                            foundMatch = false;
                            break;
                        case SearchResult::Matching:
                            matchedInternalParamsCount++;
                            break;
                    }
                    break;
                case SearchResult::NotMatching:
                    if (desc.override) {
                        foundMatch = false;
                        break;
                    }

                    switch (matchesInternal) {
                        default:
                        case SearchResult::NoParam:
                            foundMatch = false;
                            break;

                        case SearchResult::NotMatching:
                            foundMatch = false;
                            break;

                        case SearchResult::Matching:
                            matchedSearchParamsCount++;
                            matchedInternalParamsCount++;
                            break;
                    }
                    break;

                case SearchResult::Matching:
                    switch (matchesInternal) {
                        default:
                        case SearchResult::NoParam:
                            matchedSearchParamsCount++;
                            break;

                        case SearchResult::NotMatching:
                            if (desc.override) {
                                matchedSearchParamsCount++;
                                matchedInternalParamsCount++;
                            } else {
                                foundMatch = false;
                            }
                            break;

                        case SearchResult::Matching:
                            matchedSearchParamsCount++;
                            matchedInternalParamsCount++;
                            break;
                    }
            }

            if (!foundMatch) {
                break;
            }
        }

        if (!foundMatch) {
            continue;
        }

        if (desc.variation) {
            const auto &searchVarParams = desc.variation->getParams();
            if (matchedSearchParamsCount != static_cast<uint32_t>(searchVarParams.length())) {
                continue;
            }
        }

        if (matchedInternalParamsCount != static_cast<uint32_t>(internalVarParams.length())) {
            continue;
        }

        if (currentScore < bestTechniqueScore) {
            bestTechniqueIdx = i;
            bestTechniqueScore = currentScore;
        }
    }

    return bestTechniqueIdx;
}

void Material::updateParamsSet(Material::GpuParamsSetType *paramsSet, float t, bool updateAll) {
    paramsSet->update(mParams, t, updateAll);
}

Material::GpuParamsSetType *Material::createParamsSet(uint32_t techniqueIdx) {
    if (techniqueIdx >= static_cast<uint32_t>(mTechniqueList.length())) {
        return nullptr;
    }

    auto technique = mTechniqueList[techniqueIdx];
    return q_new<GpuParamsSet>(technique, mShader, mParams);
}

uint32_t Material::getPassesCount(uint32_t techniqueIdx) const {
    if (mShader == nullptr) {
        return 0;
    }

    if (techniqueIdx >= static_cast<uint32_t>(mTechniqueList.length())) {
        return static_cast<uint32_t>(0);
    }

    return static_cast<uint32_t>(mTechniqueList[techniqueIdx]->getPassesCount());
}

Material::PassType *Material::getPass(uint32_t passIdx, uint32_t techniqueIdx) const {
    if (mShader == nullptr) {
        return nullptr;
    }

    if (techniqueIdx >= static_cast<uint32_t>(mTechniqueList.length())) {
        return nullptr;
    }

    if (passIdx >= static_cast<uint32_t>(mTechniqueList[techniqueIdx]->getPassesCount())) {
        return nullptr;
    }

    return mTechniqueList[techniqueIdx]->getPass(passIdx);
}

uint32_t Material::getDefaultTechnique() const {
    FindTechniqueDesc desc{};
    return findTechnique(desc);
}

template<typename T>
void Material::getParam(const String &name, TMaterialDataParam<T> &output) const {
    output = TMaterialDataParam<T>(name, const_cast<Material *>(this));
}

template DLL_EXPORT void Material::getParam(const String &, TMaterialDataParam<float> &) const;
template DLL_EXPORT void Material::getParam(const String &, TMaterialDataParam<int> &) const;
template DLL_EXPORT void Material::getParam(const String &, TMaterialDataParam<Color> &) const;
template DLL_EXPORT void Material::getParam(const String &, TMaterialDataParam<Vector2> &) const;
template DLL_EXPORT void Material::getParam(const String &, TMaterialDataParam<Vector3> &) const;
template DLL_EXPORT void Material::getParam(const String &, TMaterialDataParam<Matrix4> &) const;

MaterialParamStruct Material::getParamStruct(const String &name) const {
    return MaterialParamStruct(name, const_cast<Material *>(this));
}

MaterialParamTexture Material::getParamTexture(const String &name) const {
    return MaterialParamTexture(name, const_cast<Material *>(this));
}

MaterialParamBuffer Material::getParamBuffer(const String &name) const {
    return MaterialParamBuffer(name, const_cast<Material *>(this));
}

MaterialParamSamplerState Material::getParamSamplerState(const String &name) const {
    return MaterialParamSamplerState(name, const_cast<Material *>(this));
}

void Material::initializeTechniques() {
    mTechniqueList.clear();

    if (mShader != nullptr) {
        mParams = new MaterialParamsType(mShader, 0);
        mTechniqueList = mShader->getCompatibleTechniques();

        if (mTechniqueList.empty()) {
            return;
        }

        initDefaultParameters();
    } else {
        mParams = nullptr;
    }
}

void Material::initDefaultParameters() {
    const auto &dataParams = mShader->getDataParams();

    for (auto &paramData : dataParams) {
        if (paramData.value.defaultValueIdx == static_cast<uint32_t>(-1)) {
            continue;
        }

        auto buffer = static_cast<uint8_t *>(mShader->getDefaultValue(paramData.value.defaultValueIdx));
        if (buffer == nullptr) {
            continue;
        }
    }

    const auto &textureParams = mShader->getTextureParams();
    for (auto &param : textureParams) {
        if (param.value.defaultValueIdx == static_cast<uint32_t>(-1)) {
            continue;
        }

        auto defaultTex = mShader->getDefaultTexture(param.value.defaultValueIdx);
    }

    const auto &samplerParams = mShader->getSamplerParams();
    for (auto &param : samplerParams) {
        if (param.value.defaultValueIdx == static_cast<uint32_t>(-1)) {
            continue;
        }

        auto defaultsampler = mShader->getDefaultSampler(param.value.defaultValueIdx);
    }
}

void Material::throwIfNotInitialized() const {
    if (mShader == nullptr) {
        EXCEPT(FLogMaterial, InternalErrorException, TEXT("Material does not have shader set."));
    }

    if (mTechniqueList.empty()) {
        EXCEPT(FLogMaterial, InternalErrorException, TEXT("Shader does not contain a supported technique."));
    }
}

void Material::setParams(MaterialParams *params) {
    if (params == nullptr) {
        return;
    }

    std::function<void(MaterialParams *, Material*, const String &, const MaterialParams::ParamData &, uint32_t)> copyParamLookup[static_cast<uint32_t>(EGpuParamDataType::Count)];

    copyParamLookup[static_cast<uint32_t>(EGpuParamDataType::Float1)] = &copyParam<float>;
    copyParamLookup[static_cast<uint32_t>(EGpuParamDataType::Float2)] = &copyParam<Vector2>;
    copyParamLookup[static_cast<uint32_t>(EGpuParamDataType::Float3)] = &copyParam<Vector3>;

    copyParamLookup[static_cast<uint32_t>(EGpuParamDataType::Int1)] = &copyParam<int>;

    copyParamLookup[static_cast<uint32_t>(EGpuParamDataType::Matrix4x4)] = &copyParam<Matrix4>;

    copyParamLookup[static_cast<uint32_t>(EGpuParamDataType::Bool)] = &copyParam<int>;
    copyParamLookup[static_cast<uint32_t>(EGpuParamDataType::Color)] = &copyParam<Color>;

    auto &dataParams = mShader->getDataParams();
    for (auto &param : dataParams) {
        uint32_t arraySize = param.value.arraySize > 1 ? param.value.arraySize : 1;
        const MaterialParams::ParamData *paramData = nullptr;

        auto result = params->getParamData(param.key, MaterialParams::EParamType::Data, param.value.type,
                                           0, &paramData);

        if (result != MaterialParams::EGetParamResult::Success) {
            continue;
        }

        uint32_t elemsToCopy = std::min(arraySize, paramData->arraySize);
        auto &copyFunc = copyParamLookup[static_cast<uint32_t>(param.value.type)];
        if (copyFunc != nullptr) {
            copyFunc(params, const_cast<Material *>(this), param.key, *paramData, elemsToCopy);
        } else {
            if (param.value.type == EGpuParamDataType::Struct) {
                auto curParam = getParamStruct(param.key);
                uint32_t structSize = params->getStructSize(*paramData);
                if (param.value.elementSize != structSize) {
                    continue;
                }

                auto structData = q_alloc<uint8_t>(structSize);
                for (uint32_t i = 0; i < elemsToCopy; i++) {
                    params->getStructData(*paramData, structData, structSize, i);
                    curParam.set(structData, structSize, i);
                }

                q_free(structData);
            }
        }
    }

    auto &textureParams = mShader->getTextureParams();
    for (auto &param : textureParams) {
        const MaterialParams::ParamData *paramData = nullptr;
        auto result = params->getParamData(param.key, MaterialParams::EParamType::Texture,
                                           EGpuParamDataType::Unknown, 0, &paramData);

        if (result != MaterialParams::EGetParamResult::Success) {
            continue;
        }

        auto texType = params->getTextureType(*paramData);
        if (texType != EMaterialParamTextureType::Normal) {
            return;
        }

        auto curParam = getParamTexture(param.key);

        TextureType texture;
        TextureSurface surface;

        params->getTexture(*paramData, texture, surface);
        curParam.set(texture);
    }

    auto &bufferParams = mShader->getBufferParams();
    for (auto &param : bufferParams) {
        const MaterialParams::ParamData *paramData = nullptr;
        auto result = params->getParamData(param.key, MaterialParams::EParamType::Buffer,
                                           EGpuParamDataType::Unknown, 0, &paramData);

        if (result != MaterialParams::EGetParamResult::Success) {
            continue;
        }

        auto curParam = getParamBuffer(param.key);
        BufferType buffer;
        params->getBuffer(*paramData, buffer);
        curParam.set(buffer);
    }

    auto &samplerParams = mShader->getSamplerParams();
    for (auto &param : samplerParams) {
        const MaterialParams::ParamData *paramData = nullptr;
        auto result = params->getParamData(param.key, MaterialParams::EParamType::Buffer,
                                           EGpuParamDataType::Unknown, 0, &paramData);

        if (result != MaterialParams::EGetParamResult::Success) {
            continue;
        }

        auto curParam = getParamSamplerState(param.key);

        SamplerStateType sampler;
        params->getSamplerState(*paramData, sampler);
        curParam.set(sampler);
    }
}
