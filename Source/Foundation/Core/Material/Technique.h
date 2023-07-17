#pragma once

#include "CorePrerequisites.h"
#include "Pass.h"
#include "ShaderVariation.h"
#include "Technique.g.h"

QCLASS()
class DLL_EXPORT Technique : public Object {
    GENERATED_BODY();

public:
    using PassType = Pass *;

protected:
    QPROPERTY()
    String mLanguage;

    QPROPERTY()
    TArray<String> mTags;

    QPROPERTY()
    ShaderVariation mVariation;

    QPROPERTY()
    TArray<PassType> mPasses;
    
public:
    Technique() = default;

    Technique(const String &language, const TArray<String> &tags, const ShaderVariation &variation, const TArray<PassType> &passes);
    ~Technique();

public:
    static Technique *New(const String &language, const TArray<PassType> &passes);
    static Technique *New(const String &language, const TArray<String> &tags, const ShaderVariation &variation, const TArray<PassType> &passes);

public:
    void compile();

    bool isSupported() const;

    bool hasTag(const String &tag);
    uint32_t hasTags() const { return !mTags.empty(); }

    const ShaderVariation &getVariation() const { return mVariation; }
    PassType getPass(uint32_t idx) const;
    uint32_t getPassesCount() const { return static_cast<uint32_t>(mPasses.length()); }

};