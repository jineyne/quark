#pragma once

#include "FIPrerequisites.h"
#include "Importer/SpecificImporter.h"

#include <FreeImage.h>

class FreeImgImporter : public SpecificImporter {
private:
    TArray<String> mExtensions;

public:
    FreeImgImporter();
    virtual ~FreeImgImporter();

public:
    virtual bool isExtensionSupported(const String &ext) const override;
    virtual bool isMagicNumberSupported(const uint8_t *magicNumPtr, uint32_t numBytes) const override;

    virtual Resource *import(const Path &path, const TSharedPtr<ImporterOptions> &options) override;
};