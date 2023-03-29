#include "Importer.h"
#include "FileSystem/FileSystem.h"
#include "Resource/Resources.h"

DEFINE_LOG_CATEGORY(FLogImporter)

FImporter::FImporter() {

}

FImporter::~FImporter() {
    for (auto &importer : mAssetImporters) {
        if (importer != nullptr)
            q_delete(importer);
    }

    mAssetImporters.clear();
}

FResourceHandle<FResource> FImporter::import(const FPath &path, const TSharedPtr<FImporterOptions> &options, const FUuid &uuid) {
    auto *importer = prepareForImport(path);
    if (importer == nullptr) {
        return nullptr;
    }

    auto output = importer->import(path, options);

    if (uuid.empty()) {
        return gResources().createResourceHandle(output);
    }

    return gResources().createResourceHandle(output, uuid);
}

FMultiResource *FImporter::importAll(const FPath &path, const TSharedPtr<FImporterOptions> &options) {
    TArray<FSubResource> output;

    TArray<SubResourceRaw> importedResource = importAllInternal(path, options);
    for (auto &entry : importedResource) {
        output.add({ entry.name, entry.value });
    }

    return q_new<FMultiResource>(output);
}

bool FImporter::supportsFileType(const FString &extension) const {
    for (const auto &importer : mAssetImporters) {
        if (importer != nullptr && importer->isExtensionSupported(extension)) {
            return true;
        }
    }

    return false;
}

void FImporter::registerAssetImporter(FSpecificImporter *importer) {
    if (!importer) {
        LOG(FLogImporter, Warning,  TEXT("Trying to register a null asset importer!"));
        return;
    }

    mAssetImporters.add(importer);
}

FSpecificImporter *FImporter::getImporterForFile(const FPath &path) const {
    FString ext = path.getExtension();
    if (ext.empty()) {
        return nullptr;
    }

    if (!supportsFileType(ext)) {
        LOG(FLogImporter, Warning, TEXT("There is no importer for the provided file type. (%ls)"), *path.toString());
        return nullptr;
    }

    for (const auto &importer : mAssetImporters) {
        if (importer != nullptr && importer->isExtensionSupported(ext)) {
            return importer;
        }
    }

    return nullptr;
}

FSpecificImporter *FImporter::prepareForImport(const FPath &path) const {
    if (!FFileSystem::IsFile(path)) {
        LOG(FLogImporter, Warning, TEXT("Trying to import asset that doesn't exists. Asset path: {0}"), path);
    }

    auto *importer = getImporterForFile(path);
    if (importer == nullptr) {
        return nullptr;
    }

    return importer;
}

TArray<SubResourceRaw> FImporter::importAllInternal(const FPath &path, const TSharedPtr<FImporterOptions> &options) {
    auto *importer = prepareForImport(path);
    if (importer == nullptr) {
        return {};
    }

    return importer->importAll(path, options);
}

FImporter &gImporter() {
    return FImporter::Instance();
}
