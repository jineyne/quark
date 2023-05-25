#pragma once

#include "FTPrerequisites.h"
#include "Importer/SpecificImporter.h"

class PLUGIN_EXPORT FFTImporter : public FSpecificImporter {
private:
    FT_Library mLibrary;
    TMap<FPath, FResource *> mCachedFont;

    TArray<FString> mExtensions;

public:
    FFTImporter();
    ~FFTImporter();

public:
    FResource *import(const FPath &path, const TSharedPtr<FImporterOptions> &options) override;

    bool isExtensionSupported(const FString &ext) const override;
    bool isMagicNumberSupported(const uint8_t *magicNumPtr, uint32_t numBytes) const override;
};