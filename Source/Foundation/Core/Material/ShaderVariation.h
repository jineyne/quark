#pragma once

#include "CorePrerequisites.h"
#include "ShaderVariation.g.h"

class DLL_EXPORT ShaderDefines {
protected:
    TMap<String, String> mDefines;

public:
    void set(const String &name, float value);
    void set(const String &name, int32_t value);
    void set(const String &name, uint32_t value);
    void set(const String &name, const String &value);

    TMap<String, String> getAll() const { return mDefines; }

    void clear() { mDefines.reset(); }
};

QSTRUCT()
struct DLL_EXPORT ShaderVariation {
    GENERATED_BODY()

public:
    enum class ParamType {
        Int,
        UInt,
        Float,
        Bool
    };

    //
    struct Param {
        union {
            int32_t i;
            uint32_t ui;
            float f;
        };

        String name;
        ParamType type;

        Param() : i(0), type(ParamType::Int) {}

        Param(const String &name, int32_t val) : i(val), name(name), type(ParamType::Int) {}
        Param(const String &name, uint32_t val) : ui(val), name(name), type(ParamType::UInt) {}
        Param(const String &name, float val) : f(val), name(name), type(ParamType::Float) {}
        Param(const String &name, bool val) : i(val ? 1 : 0), name(name), type(ParamType::Bool) {}
    };

    using ParamMap = TMap<String, Param>;

public:
    static const ShaderVariation Empty;

private:
    ParamMap mParams;
    mutable uint32_t mIdx = static_cast<uint32_t>(-1);

public:
    ShaderVariation() = default;
    ShaderVariation(const TArray<Param> &params);

public:
    bool operator==(const ShaderVariation &rhs) const;

public:
    int32_t getInt(const String &name);
    void setInt(const String &name, int32_t value);

    uint32_t getUInt(const String &name);
    void setUInt(const String &name, uint32_t value);

    float getFloat(const String &name);
    void setFloat(const String &name, float value);

    bool getBool(const String &name);
    void setBool(const String &name, bool value);

    void addParam(const Param &param) { mParams[param.name] = param; }
    void removeParam(const String &paramName) { mParams.remove(paramName); }

    bool hasParam(const String &paramName) { return mParams.find(paramName) != nullptr; }

    void clearParams() { mParams.reset(); }

    TArray<String> getParamNames() const;

    bool matches(const ShaderVariation &other, bool exact = true) const;

    const ParamMap &getParams() const { return mParams; }
    ShaderDefines getDefines() const;

    uint32_t getIdx() const { return mIdx; }
    void setIdx(uint32_t idx) const { mIdx = idx; }
};

QSTRUCT()
struct DLL_EXPORT FShaderVariations {
    GENERATED_BODY();

private:
    TArray<ShaderVariation> mVariations;
    uint32_t mNextIdx;

public:
    void add(const ShaderVariation &variation);
    const ShaderVariation &get(uint32_t idx) { return mVariations[idx]; }

    uint32_t find(const ShaderVariation &variation) const;

    const TArray<ShaderVariation> &getVariations() const { return mVariations; }
};