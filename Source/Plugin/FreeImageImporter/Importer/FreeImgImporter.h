#pragma once

#include "FIPrerequisites.h"
#include "Importer/SpecificImporter.h"

#include <FreeImage.h>

class FFreeImgImporter : public FSpecificImporter {
private:
    TArray<FString> mExtensions;

public:
    FFreeImgImporter();
    virtual ~FFreeImgImporter();

public:
    virtual bool isExtensionSupported(const FString &ext) const override;
    virtual bool isMagicNumberSupported(const uint8_t *magicNumPtr, uint32_t numBytes) const override;

    virtual FResource *import(const FPath &path, const TSharedPtr<FImporterOptions> &options) override;
};