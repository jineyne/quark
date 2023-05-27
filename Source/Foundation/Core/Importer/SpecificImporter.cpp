#include "SpecificImporter.h"

TArray<SubResourceRaw> SpecificImporter::importAll(const Path &path, const TSharedPtr<ImporterOptions> &options) {
    auto resource = import(path, options);
    if (resource == nullptr) {
        return TArray<SubResourceRaw>();
    }

    return { { TEXT("primary"), resource } };
}
