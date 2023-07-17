#include "Technique.h"
#include "Exception/Exception.h"

Technique::Technique(const String &language, const TArray<String> &tags, const ShaderVariation &variation,
                     const TArray<PassType> &passes)
        : mLanguage(language), mTags(tags), mVariation(variation), mPasses(passes) {}

Technique::~Technique() {
    mPasses.clear();
}

Technique *Technique::New(const String &language, const TArray<PassType> &passes) {
    return new (q_alloc<Technique>()) Technique(language, {}, ShaderVariation(), passes);
}

Technique *Technique::New(const String &language, const TArray<String> &tags, const ShaderVariation &variation,
                          const TArray<PassType> &passes) {
    return new (q_alloc<Technique>()) Technique(language, tags, variation, passes);
}

void Technique::compile() {
    for (auto &pass : mPasses) {
        pass->compile();
    }
}

bool Technique::isSupported() const {
    return true;
}

bool Technique::hasTag(const String &tag) {
    for (auto &entry : mTags) {
        if (entry == tag) {
            return true;
        }
    }

    return false;
}

Technique::PassType Technique::getPass(uint32_t idx) const {
    if (idx < 0 || idx >= static_cast<uint32_t>(mPasses.length())) {
        EXCEPT(LogMaterial, InvalidParametersException, TEXT("Index out of range: %d"), idx);
    }

    return mPasses[idx];
}
