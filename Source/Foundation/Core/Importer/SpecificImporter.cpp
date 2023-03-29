#include "SpecificImporter.h"

TArray<SubResourceRaw> FSpecificImporter::importAll(const FPath &path, const TSharedPtr<FImporterOptions> &options) {
    auto resource = import(path, options);
    if (resource == nullptr) {
        return TArray<SubResourceRaw>();
    }

    return { { TEXT("primary"), resource } };
}
