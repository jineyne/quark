#include "FreeTypeFont.h"

FreeTypeFont::FreeTypeFont(const FT_Face &face) : mFace(face) {

}

FreeTypeFont::~FreeTypeFont() {
    q_delete(mSource);
    FT_Done_Face(mFace);
}

void FreeTypeFont::setFontSize(Size size) {
    FT_Set_Pixel_Sizes(mFace, size.width, size.height);
}

void FreeTypeFont::setSource(uint8_t *source) {
    mSource = std::move(source);
}

int FreeTypeFont::getAscender() const {
    return mFace->size->metrics.ascender;
}

int FreeTypeFont::getDescender() const {
    return mFace->size->metrics.descender;
}

int FreeTypeFont::getHeight() const {
    return mFace->size->metrics.height;
}

bool FreeTypeFont::makeGlyphInternal(const Glyph &glyph, GlyphData &data) {
    FT_Set_Pixel_Sizes(mFace, glyph.size, glyph.size);

    auto ix = FT_Get_Char_Index(mFace, glyph.ch);
    if (ix == 0) {
        return false;
    }

    if (FT_Load_Glyph(mFace, ix, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP)) {
        return false;
    }

    data = GlyphData{};
    data.bitmap = Size(mFace->glyph->bitmap.width, mFace->glyph->bitmap.rows);
    auto emptySpace = findEmptySpace(Size((data.bitmap.width + 3) / 4, (data.bitmap.height + 3) / 4));
    if (emptySpace.x < 0) {
        clearUnusedSpace();
        emptySpace = findEmptySpace(Size((data.bitmap.width + 3) / 4, (data.bitmap.height + 3) / 4));
        if (emptySpace.x < 0) {
            return false;
        }
    }

    data.advance = Vector2(mFace->glyph->advance.x >> 6, mFace->glyph->advance.y >> 6);
    data.height = mFace->glyph->metrics.height >> 6;
    data.left = mFace->glyph->bitmap_left;
    data.top = mFace->glyph->bitmap_top;
    data.u = emptySpace.x * 4 / mTextureSize.width;
    data.v = emptySpace.y * 4 / mTextureSize.height;
    data.w = data.bitmap.width / mTextureSize.width;
    data.h = data.bitmap.height / mTextureSize.height;
    data.use = 0;

    mCachedGlyphDataMap.add(glyph, data);

    for (uint32_t j = 0; j < data.bitmap.height; j++) {
        for (uint32_t i = 0; i < data.bitmap.width; i++) {
            uint8_t a = mFace->glyph->bitmap.buffer[i + j * (uint32_t)data.bitmap.width];
            Color color = Color::White;
            color.setAlpha((a == 0 ? a : (float)a / (float)255));

            mPixelData->setColorAt(color, (i + emptySpace.x * 4), (j + emptySpace.y * 4));
        }
    }

    fillSpace(Rect(emptySpace.x, emptySpace.y, (data.bitmap.width + 3) / 4, (data.bitmap.height + 3) / 4), glyph.ch);

    mCachedGlyphDataMap.add(glyph, data);
    return true;
}
