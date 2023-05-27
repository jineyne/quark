#pragma once

#include "FTPrerequisites.h"
#include "Importer/SpecificImporter.h"

class PLUGIN_EXPORT FreeTypeImporter : public SpecificImporter {
private:
    FT_Library mLibrary;
    TMap<Path, Resource *> mCachedFont;

    TArray<String> mExtensions;

public:
    FreeTypeImporter();
    ~FreeTypeImporter();

public:
    Resource *import(const Path &path, const TSharedPtr<ImporterOptions> &options) override;

    bool isExtensionSupported(const String &ext) const override;
    bool isMagicNumberSupported(const uint8_t *magicNumPtr, uint32_t numBytes) const override;
};