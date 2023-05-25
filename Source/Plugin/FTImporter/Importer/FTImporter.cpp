#include "FTImporter.h"

#include "Importer/Importer.h"
#include "Exception/Exception.h"

#include "Text/FTFont.h"
#include "FileSystem/FileSystem.h"

FFTImporter::FFTImporter() {
    mExtensions.add(TEXT("ttf"));

    if (FT_Init_FreeType(&mLibrary)) {
        EXCEPT(FLogImporter, InternalErrorException, TEXT("Failed to initialize font engine."));
    }
}

FFTImporter::~FFTImporter() {
    for (auto it : mCachedFont) {
        auto ftfont = static_cast<FFTFont *>(it.value);
        FT_Done_Face(ftfont->getFace());
    }

    FT_Done_FreeType(mLibrary);
}

FResource *FFTImporter::import(const FPath &path, const TSharedPtr<FImporterOptions> &options) {
    auto it = mCachedFont.find(path);
    if (it != nullptr) {
        return *it;
    }

    if (!FFileSystem::Exists(path)) {
        LOG(FLogImporter, Error, TEXT("Unable to find font: %ls"), *path.toString());
        return nullptr;
    }

    auto file = FFileSystem::OpenFile(path);
    auto *source = q_alloc<uint8_t>(file->size());
    file->read(source, file->size());

    FT_Face face;
    if (FT_New_Memory_Face(mLibrary, source, file->size(), 0, &face)) {
        LOG(FLogImporter, Error, TEXT("Unable to read font: %ls"), *path.toString());
        return nullptr;
    }

    auto font = q_new<FFTFont>(face);
    mCachedFont.add(path, static_cast<FResource *>(font));

    const FString fileName = path.getFilename();
    font->setName(fileName);

    font->setSource(source);

    return font;
}

bool FFTImporter::isExtensionSupported(const FString &ext) const {
    FString lowerCastExt = ext.lower();
    return std::find(mExtensions.begin(), mExtensions.end(), lowerCastExt) != mExtensions.end();
}

bool FFTImporter::isMagicNumberSupported(const uint8_t *magicNumPtr, uint32_t numBytes) const {
    return true;
}
