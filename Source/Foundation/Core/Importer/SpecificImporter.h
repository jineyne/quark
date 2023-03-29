#pragma once

#include "CorePrerequisites.h"
#include "Resource/Resource.h"
#include "ImporterOptions.h"

struct SubResourceRaw {
    FString name;
    FResource *value;
};


class DLL_EXPORT FSpecificImporter {

public:
    FSpecificImporter() = default;
    virtual ~FSpecificImporter() = default;

public:
    virtual bool isExtensionSupported(const FString &ext) const = 0;
    virtual bool isMagicNumberSupported(const uint8_t *magicNumPtr, uint32_t numBytes) const = 0;

    virtual FResource *import(const FPath &path, const TSharedPtr<FImporterOptions> &options) = 0;
    virtual TArray<SubResourceRaw> importAll(const FPath &path, const TSharedPtr<FImporterOptions> &options);
};