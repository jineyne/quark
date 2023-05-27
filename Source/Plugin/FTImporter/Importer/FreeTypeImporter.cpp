#include "FreeTypeImporter.h"

#include "Importer/Importer.h"
#include "Exception/Exception.h"

#include "Text/FreeTypeFont.h"
#include "FileSystem/FileSystem.h"

FreeTypeImporter::FreeTypeImporter() {
    mExtensions.add(TEXT("ttf"));

    if (FT_Init_FreeType(&mLibrary)) {
        EXCEPT(LogImporter, InternalErrorException, TEXT("Failed to initialize font engine."));
    }
}

FreeTypeImporter::~FreeTypeImporter() {
    for (auto it : mCachedFont) {
        auto ftfont = static_cast<FreeTypeFont *>(it.value);
        FT_Done_Face(ftfont->getFace());
    }

    FT_Done_FreeType(mLibrary);
}

Resource *FreeTypeImporter::import(const Path &path, const TSharedPtr<ImporterOptions> &options) {
    auto it = mCachedFont.find(path);
    if (it != nullptr) {
        return *it;
    }

    if (!FileSystem::Exists(path)) {
        LOG(LogImporter, Error, TEXT("Unable to find font: %ls"), *path.toString());
        return nullptr;
    }

    auto file = FileSystem::OpenFile(path);
    auto *source = q_alloc<uint8_t>(file->size());
    file->read(source, file->size());

    FT_Face face;
    if (FT_New_Memory_Face(mLibrary, source, file->size(), 0, &face)) {
        LOG(LogImporter, Error, TEXT("Unable to read font: %ls"), *path.toString());
        return nullptr;
    }

    auto font = q_new<FreeTypeFont>(face);
    mCachedFont.add(path, static_cast<Resource *>(font));

    const String fileName = path.getFilename();
    font->setName(fileName);

    font->setSource(source);

    return font;
}

bool FreeTypeImporter::isExtensionSupported(const String &ext) const {
    String lowerCastExt = ext.lower();
    return std::find(mExtensions.begin(), mExtensions.end(), lowerCastExt) != mExtensions.end();
}

bool FreeTypeImporter::isMagicNumberSupported(const uint8_t *magicNumPtr, uint32_t numBytes) const {
    return true;
}
