#include "Technique.h"
#include "Exception/Exception.h"

FTechnique::FTechnique(const FString &language, const TArray<FString> &tags, const FShaderVariation &variation,
                       const TArray<PassType> &passes)
        : mLanguage(language), mTags(tags), mVariation(variation), mPasses(passes) {}

FTechnique::~FTechnique() {
    mPasses.clear();
}

FTechnique *FTechnique::New(const FString &language, const TArray<PassType> &passes) {
    return new (q_alloc<FTechnique>()) FTechnique(language, {}, FShaderVariation(), passes);
}

FTechnique *FTechnique::New(const FString &language, const TArray<FString> &tags, const FShaderVariation &variation,
                            const TArray<PassType> &passes) {
    return new (q_alloc<FTechnique>()) FTechnique(language, tags, variation, passes);
}

void FTechnique::compile() {
    for (auto &pass : mPasses) {
        pass->compile();
    }
}

bool FTechnique::isSupported() const {
    return true;
}

bool FTechnique::hasTag(const FString &tag) {
    for (auto &entry : mTags) {
        if (entry == tag) {
            return true;
        }
    }

    return false;
}

FTechnique::PassType FTechnique::getPass(uint32_t idx) const {
    if (idx < 0 || idx >= static_cast<uint32_t>(mPasses.length())) {
        EXCEPT(FLogMaterial, InvalidParametersException, TEXT("Index out of range: %d"), idx);
    }

    return mPasses[idx];
}
