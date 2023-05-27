#include "ShaderVariation.h"

void ShaderDefines::set(const String &name, float value) {
    mDefines[name] = String::ToString(value);
}

void ShaderDefines::set(const String &name, int32_t value) {
    mDefines[name] = String::ToString(value);
}

void ShaderDefines::set(const String &name, uint32_t value) {
    mDefines[name] = String::ToString(value);
}

void ShaderDefines::set(const String &name, const String &value) {
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

int32_t FShaderVariation::getInt(const String &name) {
    auto it = mParams.find(name);

    if (it == nullptr) {
        return 0;
    } else {
        return (*it).i;
    }
}

void FShaderVariation::setInt(const String &name, int32_t value) {
    addParam(Param(name, value));
}

uint32_t FShaderVariation::getUInt(const String &name) {
    auto it = mParams.find(name);

    if (it == nullptr) {
        return 0;
    } else {
        return (*it).ui;
    }
}

void FShaderVariation::setUInt(const String &name, uint32_t value) {
    addParam(Param(name, value));
}

float FShaderVariation::getFloat(const String &name) {
    auto it = mParams.find(name);

    if (it == nullptr) {
        return 0;
    } else {
        return (*it).f;
    }
}

void FShaderVariation::setFloat(const String &name, float value) {
    addParam(Param(name, value));
}

bool FShaderVariation::getBool(const String &name) {
    auto it = mParams.find(name);

    if (it == nullptr) {
        return 0;
    } else {
        return (*it).i > 0 ? true : false;
    }
}

void FShaderVariation::setBool(const String &name, bool value) {
    addParam(Param(name, value));
}

TArray<String> FShaderVariation::getParamNames() const {
    TArray<String> result;
    result.reserve(mParams.length());

    for (auto &entry : mParams) {
        result.add(entry.key);
    }

    return result;
}

bool FShaderVariation::matches(const FShaderVariation &other, bool exact) const {
    for (auto &entry : other.mParams) {
        const auto it = mParams.find(entry.key);
        if (it == nullptr) {
            return false;
        }

        if (entry.value.i != (*it).i) {
            return false;
        }
    }

    if (exact) {
        for (auto &entry : mParams) {
            const auto it = other.mParams.find(entry.key);
            if (it == nullptr) {
                return false;
            }

            if (entry.value.i != (*it).i) {
                return false;
            }
        }
    }

    return true;
}

ShaderDefines FShaderVariation::getDefines() const {
    ShaderDefines defines;
    for (auto &entry : mParams) {
        switch (entry.value.type) {
            case ParamType::Int:
            case ParamType::Bool:
                defines.set(entry.key, entry.value.i);
                break;
            case ParamType::UInt:
                defines.set(entry.key, entry.value.ui);
                break;
            case ParamType::Float:
                defines.set(entry.key, entry.value.f);
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
