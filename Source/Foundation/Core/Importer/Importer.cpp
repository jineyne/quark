#include "Importer.h"
#include "FileSystem/FileSystem.h"
#include "Resource/Resources.h"

DEFINE_LOG_CATEGORY(LogImporter)

Importer::Importer() {

}

Importer::~Importer() {
    for (auto &importer : mAssetImporters) {
        if (importer != nullptr)
            q_delete(importer);
    }

    mAssetImporters.clear();
}

FResourceHandle<Resource> Importer::import(const Path &path, const TSharedPtr<ImporterOptions> &options, const Uuid &uuid) {
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

MultiResource *Importer::importAll(const Path &path, const TSharedPtr<ImporterOptions> &options) {
    TArray<SubResource> output;

    TArray<SubResourceRaw> importedResource = importAllInternal(path, options);
    for (auto &entry : importedResource) {
        output.add({ entry.name, entry.value });
    }

    return q_new<MultiResource>(output);
}

bool Importer::supportsFileType(const String &extension) const {
    for (const auto &importer : mAssetImporters) {
        if (importer != nullptr && importer->isExtensionSupported(extension)) {
            return true;
        }
    }

    return false;
}

void Importer::registerAssetImporter(SpecificImporter *importer) {
    if (!importer) {
        LOG(LogImporter, Warning, TEXT("Trying to register a null asset importer!"));
        return;
    }

    mAssetImporters.add(importer);
}

SpecificImporter *Importer::getImporterForFile(const Path &path) const {
    String ext = path.getExtension();
    if (ext.empty()) {
        return nullptr;
    }

    if (!supportsFileType(ext)) {
        LOG(LogImporter, Warning, TEXT("There is no importer for the provided file type. (%ls)"), *path.toString());
        return nullptr;
    }

    for (const auto &importer : mAssetImporters) {
        if (importer != nullptr && importer->isExtensionSupported(ext)) {
            return importer;
        }
    }

    return nullptr;
}

SpecificImporter *Importer::prepareForImport(const Path &path) const {
    if (!FileSystem::IsFile(path)) {
        LOG(LogImporter, Warning, TEXT("Trying to import asset that doesn't exists. Asset path: %ls"), *path.toString());
    }

    auto *importer = getImporterForFile(path);
    if (importer == nullptr) {
        return nullptr;
    }

    return importer;
}

TArray<SubResourceRaw> Importer::importAllInternal(const Path &path, const TSharedPtr<ImporterOptions> &options) {
    auto *importer = prepareForImport(path);
    if (importer == nullptr) {
        return {};
    }

    return importer->importAll(path, options);
}

Importer &gImporter() {
    return Importer::Instance();
}
