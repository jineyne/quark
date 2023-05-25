#include "Font.h"

void FFont::flush() {
    mTexture->writeData(mPixelData, 0, 0, true);
}

FGlyphData FFont::getGlyph(const FGlyph &glyph) {
    auto it = mCachedGlyphDataMap.find(glyph);
    if (it != nullptr) {
        return *it;
    }

    FGlyphData data{};
    makeGlyphInternal(glyph, data);

    return data;
}

void FFont::initialize() {
    mCachedTextureMap.resize((mTextureSize.width / 4) * (mTextureSize.height / 4));

    FTextureDesc textureDesc{};
    textureDesc.format = EPixelFormat::RGBA8;
    textureDesc.type = ETextureType::e2D;
    textureDesc.usage = ETextureUsage::Dynamic;
    textureDesc.width = mTextureSize.width;
    textureDesc.height = mTextureSize.height;
    textureDesc.depth = 4;
    textureDesc.mipLevels = 1;
    mTexture = FTexture::NewPtr(textureDesc);
    mTexture->setName(TEXT("FontCachedTexture"));

    mPixelData = FPixelData::New(mTexture->getWidth(), mTexture->getHeight(), mTexture->getDepth(), mTexture->getFormat());
    mPixelData->allocateInternalBuffer();
    mPixelData->setColors(FColor(0, 0, 0, 0));

    FResource::initialize();
}

bool FFont::testSpace(const FRect &space) const {
    if (space.right() >= mTexture->getWidth() / 4) {
        return false;
    }

    if (space.top() >= mTexture->getHeight() / 4) {
        return false;
    }

    for (uint32_t i = space.x; i < space.right(); i++) {
        for (uint32_t j = space.y; j < space.top(); j++) {
            if (mapAt(i, j)) {
                return false;
            }
        }
    }


    return true;
}

void FFont::fillSpace(const FRect &space, TCHAR ch) {
    for (uint32_t i = space.x; i < space.right(); i++) {
        for (uint32_t j = space.y; j < space.top(); j++) {
            mapAt(i, j) = ch;
        }
    }
}

FVector2 FFont::findEmptySpace(FSize size) {
    for (uint32_t j = 0; j < mTextureSize.height / 4; j++) {
        for (uint32_t i = 0; i < mTextureSize.width / 4; i++) {
            if (testSpace(FRect(i, j, size.width, size.height))) {
                return FVector2(i, j);
            }
        }
    }

    return FVector2(-1, -1);
}

void FFont::clearUnusedSpace() {
    TMap<FGlyph, FGlyphData> newMap;
    for (auto data : mCachedGlyphDataMap) {
        if (data.value.u == 1) {
            newMap.add(data.key, data.value);
        } else {
            fillSpace(FRect(data.value.u * mTextureSize.width / 4, data.value.v * mTextureSize.height / 4,
                           (data.value.bitmap.width + 3) / 4, (data.value.bitmap.height + 3) / 4), 0);
        }
    }

    // TODO: Swap
    mCachedGlyphDataMap = newMap;
}

const TCHAR &FFont::mapAt(int x, int y) const {
    return mCachedTextureMap[x + y * (mTextureSize.width / 4)];
}

TCHAR &FFont::mapAt(int x, int y) {
    return mCachedTextureMap[x + y * (mTextureSize.width / 4)];
}
