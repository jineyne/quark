#pragma once

#include "FTPrerequisites.h"
#include "Text/Font.h"

class PLUGIN_EXPORT FreeTypeFont : public Font {
private:
    FT_Face mFace;

    uint8_t *mSource;

public:
    FreeTypeFont(const FT_Face &face);
    ~FreeTypeFont();

public:
    void setFontSize(Size size) override;
    void setSource(uint8_t *source);

    int getAscender() const override;
    int getDescender() const override;
    int getHeight() const override;

    const auto &getFace() const { return mFace; }

protected:
    bool makeGlyphInternal(const Glyph &glyph, GlyphData &data);
};
