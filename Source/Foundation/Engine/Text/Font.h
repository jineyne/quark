#pragma once

#include "CorePrerequisites.h"
#include "Image/Texture.h"
#include "Math/Rect.h"
#include "Math/Vector2.h"
#include "Glyph.h"

class DLL_EXPORT Font : public Resource {
protected:
    Size mTextureSize = Size(1024, 1024);

    Texture *mTexture = nullptr;
    FPixelData *mPixelData = nullptr;

    TArray<TCHAR> mCachedTextureMap;

    TMap<Glyph, GlyphData> mCachedGlyphDataMap;

public:
    Font() = default;

    virtual ~Font() = default;

public:
    void flush();

    virtual void setFontSize(Size size) = 0;

    GlyphData getGlyph(const Glyph &glyph);

    const auto &getTexture() const { return mTexture; }

    virtual int getAscender() const = 0;

    virtual int getDescender() const = 0;

    virtual int getHeight() const = 0;

protected:
    void initialize() override;

    virtual bool makeGlyphInternal(const Glyph &glyph, GlyphData &data) = 0;

    bool testSpace(const Rect &space) const;

    void fillSpace(const Rect &space, TCHAR ch = 1);

    Vector2 findEmptySpace(Size size);

    void clearUnusedSpace();

    const TCHAR &mapAt(int x, int y) const;

    TCHAR &mapAt(int x, int y);
};
