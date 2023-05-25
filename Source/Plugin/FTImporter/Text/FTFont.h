#pragma once

#include "FTPrerequisites.h"
#include "Text/Font.h"

class DLL_EXPORT FFTFont : public FFont {
private:
    FT_Face mFace;

    uint8_t *mSource;

public:
    FFTFont(const FT_Face &face);
    ~FFTFont();

public:
    void setFontSize(FSize size) override;
    void setSource(uint8_t *source);

    int getAscender() const override;
    int getDescender() const override;
    int getHeight() const override;

    const auto &getFace() const { return mFace; }

protected:
    bool makeGlyphInternal(const FGlyph &glyph, FGlyphData &data);
};
