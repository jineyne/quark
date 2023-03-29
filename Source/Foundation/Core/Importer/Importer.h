#pragma once

#include "CorePrerequisites.h"
#include "Resource/Resource.h"
#include "Misc/Module.h"
#include "Importer.g.h"
#include "ImporterOptions.h"
#include "SpecificImporter.h"

DLL_EXPORT DECLARE_LOG_CATEGORY_EXTERN(FLogImporter, Debug)

struct FSubResource {
public:
    FString name;
    FResource *value;
};

struct FMultiResource {
public:
    TArray<FSubResource> entries;

public:
    FMultiResource() = default;
    FMultiResource(const TArray<FSubResource> & entries) : entries(entries) { }
};

QCLASS()
class DLL_EXPORT FImporter : public TModule<FImporter> {
    GENERATED_BODY()

private:
    TArray<FSpecificImporter *> mAssetImporters;

public:
    FImporter();
    ~FImporter();

public:
    FResourceHandle<FResource> import(const FPath &path, const TSharedPtr<FImporterOptions> &options = nullptr, const FUuid &uuid = FUuid::Empty);

    template <class T>
    FResourceHandle<T> import(const FPath &path, const TSharedPtr<FImporterOptions> &options = nullptr, const FUuid &uuid = FUuid::Empty) {
        return StaticResourceCast<T>(import(path, options, uuid));
    }

    FMultiResource *importAll(const FPath &path, const TSharedPtr<FImporterOptions> &options = nullptr);

    bool supportsFileType(const FString &extension) const;

    void registerAssetImporter(FSpecificImporter *importer);

private:
    FSpecificImporter *getImporterForFile(const FPath &path) const;
    FSpecificImporter *prepareForImport(const FPath &path) const;

    TArray<SubResourceRaw> importAllInternal(const FPath &path, const TSharedPtr<FImporterOptions> &options);
};

DLL_EXPORT FImporter &gImporter();