#pragma once

#include "Prerequisites.h"
#include "Image/Texture.h"
#include "Math/Rect.h"

class DLL_EXPORT Sprite {
private:
    FResourceHandle<Texture> mTexture;
    Rect mBound;

public:
    static Sprite *New();

public:
    const FResourceHandle<Texture> &getTexture() const;
    void setTexture(const FResourceHandle<Texture> &texture);

    const Rect &getBound() const;
    void setBound(const Rect &bound);
};
