#pragma once

#include "CorePrerequisites.h"
#include "Pass.h"
#include "ShaderVariation.h"

class DLL_EXPORT FTechnique {
public:
    using PassType = FPass *;

protected:
    FString mLanguage;
    TArray<FString> mTags;
    FShaderVariation mVariation;
    TArray<PassType> mPasses;
    
public:
    FTechnique(const FString &language, const TArray<FString> &tags, const FShaderVariation &variation, const TArray<PassType> &passes);
    ~FTechnique();

public:
    static FTechnique *New(const FString &language, const TArray<PassType> &passes);
    static FTechnique *New(const FString &language, const TArray<FString> &tags, const FShaderVariation &variation,  const TArray<PassType> &passes);

public:
    void compile();

    bool isSupported() const;

    bool hasTag(const FString &tag);
    uint32_t hasTags() const { return !mTags.empty(); }

    const FShaderVariation &getVariation() const { return mVariation; }
    PassType getPass(uint32_t idx) const;
    uint32_t getPassesCount() const { return static_cast<uint32_t>(mPasses.length()); }

};