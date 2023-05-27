#pragma once

#include "CorePrerequisites.h"
#include "Resource/Resource.h"
#include "Misc/Module.h"
#include "SpecificImporter.h"
#include "ImporterOptions.h"
#include "Importer.g.h"

DLL_EXPORT DECLARE_LOG_CATEGORY_EXTERN(LogImporter, Debug)

struct SubResource {
public:
    String name;
    Resource *value;
};

struct MultiResource {
public:
    TArray<SubResource> entries;

public:
    MultiResource() = default;
    MultiResource(const TArray<SubResource> & entries) : entries(entries) { }
};

QCLASS()
class DLL_EXPORT Importer : public TModule<Importer> {
    GENERATED_BODY()

private:
    TArray<SpecificImporter *> mAssetImporters;

public:
    Importer();
    ~Importer();

public:
    FResourceHandle<Resource> import(const Path &path, const TSharedPtr<ImporterOptions> &options = nullptr, const Uuid &uuid = Uuid::Empty);

    template <class T>
    FResourceHandle<T> import(const Path &path, const TSharedPtr<ImporterOptions> &options = nullptr, const Uuid &uuid = Uuid::Empty) {
        return StaticResourceCast<T>(import(path, options, uuid));
    }

    MultiResource *importAll(const Path &path, const TSharedPtr<ImporterOptions> &options = nullptr);

    bool supportsFileType(const String &extension) const;

    void registerAssetImporter(SpecificImporter *importer);

private:
    SpecificImporter *getImporterForFile(const Path &path) const;
    SpecificImporter *prepareForImport(const Path &path) const;

    TArray<SubResourceRaw> importAllInternal(const Path &path, const TSharedPtr<ImporterOptions> &options);
};

DLL_EXPORT Importer &gImporter();