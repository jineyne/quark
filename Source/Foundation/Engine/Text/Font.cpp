#include "Font.h"

void Font::flush() {
    mTexture->writeData(mPixelData, 0, 0, true);
}

GlyphData Font::getGlyph(const Glyph &glyph) {
    auto it = mCachedGlyphDataMap.find(glyph);
    if (it != nullptr) {
        return *it;
    }

    GlyphData data{};
    makeGlyphInternal(glyph, data);

    return data;
}

void Font::initialize() {
    mCachedTextureMap.resize((mTextureSize.width / 4) * (mTextureSize.height / 4));

    TextureDesc textureDesc{};
    textureDesc.format = EPixelFormat::RGBA8;
    textureDesc.type = ETextureType::e2D;
    textureDesc.usage = ETextureUsage::Dynamic;
    textureDesc.width = mTextureSize.width;
    textureDesc.height = mTextureSize.height;
    textureDesc.depth = 4;
    textureDesc.mipLevels = 1;
    mTexture = Texture::NewPtr(textureDesc);
    mTexture->setName(TEXT("FontCachedTexture"));

    mPixelData = FPixelData::New(mTexture->getWidth(), mTexture->getHeight(), mTexture->getDepth(), mTexture->getFormat());
    mPixelData->allocateInternalBuffer();
    mPixelData->setColors(Color(0, 0, 0, 0));

    Resource::initialize();
}

bool Font::testSpace(const Rect &space) const {
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

void Font::fillSpace(const Rect &space, TCHAR ch) {
    for (uint32_t i = space.x; i < space.right(); i++) {
        for (uint32_t j = space.y; j < space.top(); j++) {
            mapAt(i, j) = ch;
        }
    }
}

Vector2 Font::findEmptySpace(Size size) {
    for (uint32_t j = 0; j < mTextureSize.height / 4; j++) {
        for (uint32_t i = 0; i < mTextureSize.width / 4; i++) {
            if (testSpace(Rect(i, j, size.width, size.height))) {
                return Vector2(i, j);
            }
        }
    }

    return Vector2(-1, -1);
}

void Font::clearUnusedSpace() {
    TMap<Glyph, GlyphData> newMap;
    for (auto data : mCachedGlyphDataMap) {
        if (data.value.u == 1) {
            newMap.add(data.key, data.value);
        } else {
            fillSpace(Rect(data.value.u * mTextureSize.width / 4, data.value.v * mTextureSize.height / 4,
                           (data.value.bitmap.width + 3) / 4, (data.value.bitmap.height + 3) / 4), 0);
        }
    }

    // TODO: Swap
    mCachedGlyphDataMap = newMap;
}

const TCHAR &Font::mapAt(int x, int y) const {
    return mCachedTextureMap[x + y * (mTextureSize.width / 4)];
}

TCHAR &Font::mapAt(int x, int y) {
    return mCachedTextureMap[x + y * (mTextureSize.width / 4)];
}
