#include "ShaderVariation.h"

void FShaderDefines::set(const FString &name, float value) {
    mDefines[name] = FString::ToString(value);
}

void FShaderDefines::set(const FString &name, int32_t value) {
    mDefines[name] = FString::ToString(value);
}

void FShaderDefines::set(const FString &name, uint32_t value) {
    mDefines[name] = FString::ToString(value);
}

void FShaderDefines::set(const FString &name, const FString &value) {
    mDefines[name] = value;
}

const FShaderVariation FShaderVariation::Empty;

FShaderVariation::FShaderVariation(const TArray<Param> &params) {
    for (auto &entry : params) {
        mParams[entry.name] = entry;
    }
}

bool FShaderVariation::operator==(const FShaderVariation &rhs) const {
    return matches(rhs);
}

int32_t FShaderVariation::getInt(const FString &name) {
    auto it = mParams.find(name);

    if (it == nullptr) {
        return 0;
    } else {
        return (*it).i;
    }
}

void FShaderVariation::setInt(const FString &name, int32_t value) {
    addParam(Param(name, value));
}

uint32_t FShaderVariation::getUInt(const FString &name) {
    auto it = mParams.find(name);

    if (it == nullptr) {
        return 0;
    } else {
        return (*it).ui;
    }
}

void FShaderVariation::setUInt(const FString &name, uint32_t value) {
    addParam(Param(name, value));
}

float FShaderVariation::getFloat(const FString &name) {
    auto it = mParams.find(name);

    if (it == nullptr) {
        return 0;
    } else {
        return (*it).f;
    }
}

void FShaderVariation::setFloat(const FString &name, float value) {
    addParam(Param(name, value));
}

bool FShaderVariation::getBool(const FString &name) {
    auto it = mParams.find(name);

    if (it == nullptr) {
        return 0;
    } else {
        return (*it).i > 0 ? true : false;
    }
}

void FShaderVariation::setBool(const FString &name, bool value) {
    addParam(Param(name, value));
}

TArray<FString> FShaderVariation::getParamNames() const {
    TArray<FString> result;
    result.reserve(mParams.length());

    for (auto &entry : mParams) {
        result.add(entry.first);
    }

    return result;
}

bool FShaderVariation::matches(const FShaderVariation &other, bool exact) const {
    for (auto &entry : other.mParams) {
        const auto it = mParams.find(entry.first);
        if (it == nullptr) {
            return false;
        }

        if (entry.second.i != (*it).i) {
            return false;
        }
    }

    if (exact) {
        for (auto &entry : mParams) {
            const auto it = other.mParams.find(entry.first);
            if (it == nullptr) {
                return false;
            }

            if (entry.second.i != (*it).i) {
                return false;
            }
        }
    }

    return true;
}

FShaderDefines FShaderVariation::getDefines() const {
    FShaderDefines defines;
    for (auto &entry : mParams) {
        switch (entry.second.type) {
            case ParamType::Int:
            case ParamType::Bool:
                defines.set(entry.first, entry.second.i);
                break;
            case ParamType::UInt:
                defines.set(entry.first, entry.second.ui);
                break;
            case ParamType::Float:
                defines.set(entry.first, entry.second.f);
                break;
        }
    }

    return defines;
}

void FShaderVariations::add(const FShaderVariation &variation) {
    variation.setIdx(mNextIdx++);
    mVariations.add(variation);
}

uint32_t FShaderVariations::find(const FShaderVariation &variation) const {
    uint32_t idx = 0;
    for (auto &entry : mVariations) {
        if (entry == variation) {
            return idx;
        }

        idx++;
    }

    return static_cast<uint32_t>(-1);
}
