#pragma once

#include "CorePrerequisites.h"
#include "Pass.h"
#include "ShaderVariation.h"
#include "Technique.g.h"

QSTRUCT()
struct DLL_EXPORT Technique {
    GENERATED_BODY()

public:
    using PassType = Pass *;

protected:
    String mLanguage;
    TArray<String> mTags;
    FShaderVariation mVariation;
    TArray<PassType> mPasses;
    
public:
    Technique(const String &language, const TArray<String> &tags, const FShaderVariation &variation, const TArray<PassType> &passes);
    ~Technique();

public:
    static Technique *New(const String &language, const TArray<PassType> &passes);
    static Technique *New(const String &language, const TArray<String> &tags, const FShaderVariation &variation, const TArray<PassType> &passes);

public:
    void compile();

    bool isSupported() const;

    bool hasTag(const String &tag);
    uint32_t hasTags() const { return !mTags.empty(); }

    const FShaderVariation &getVariation() const { return mVariation; }
    PassType getPass(uint32_t idx) const;
    uint32_t getPassesCount() const { return static_cast<uint32_t>(mPasses.length()); }

};