#pragma once

#include "CorePrerequisites.h"
#include "Image/Texture.h"
#include "Math/Rect.h"
#include "Math/Vector2.h"
#include "Glyph.h"

class DLL_EXPORT FFont : public FResource {
protected:
    FSize mTextureSize = FSize(1024, 1024);

    FTexture *mTexture = nullptr;
    FPixelData *mPixelData = nullptr;

    TArray<TCHAR> mCachedTextureMap;

    TMap<FGlyph, FGlyphData> mCachedGlyphDataMap;

public:
    FFont() = default;

    virtual ~FFont() = default;

public:
    void flush();

    virtual void setFontSize(FSize size) = 0;

    FGlyphData getGlyph(const FGlyph &glyph);

    const auto &getTexture() const { return mTexture; }

    virtual int getAscender() const = 0;

    virtual int getDescender() const = 0;

    virtual int getHeight() const = 0;

protected:
    void initialize() override;

    virtual bool makeGlyphInternal(const FGlyph &glyph, FGlyphData &data) = 0;

    bool testSpace(const FRect &space) const;

    void fillSpace(const FRect &space, TCHAR ch = 1);

    FVector2 findEmptySpace(FSize size);

    void clearUnusedSpace();

    const TCHAR &mapAt(int x, int y) const;

    TCHAR &mapAt(int x, int y);
};
