#pragma once

#include "CorePrerequisites.h"
#include "Resource/Resource.h"
#include "ImporterOptions.h"

struct SubResourceRaw {
    String name;
    Resource *value;
};


class DLL_EXPORT SpecificImporter {

public:
    SpecificImporter() = default;
    virtual ~SpecificImporter() = default;

public:
    virtual bool isExtensionSupported(const String &ext) const = 0;
    virtual bool isMagicNumberSupported(const uint8_t *magicNumPtr, uint32_t numBytes) const = 0;

    virtual Resource *import(const Path &path, const TSharedPtr<ImporterOptions> &options) = 0;
    virtual TArray<SubResourceRaw> importAll(const Path &path, const TSharedPtr<ImporterOptions> &options);
};