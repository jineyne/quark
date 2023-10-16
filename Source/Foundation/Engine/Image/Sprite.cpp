//
// Created by jiney on 2023-10-16.
//

#include "Sprite.h"

Sprite *Sprite::New() {
    return q_new<Sprite>();
}

const FResourceHandle<Texture> &Sprite::getTexture() const {
    return mTexture;
}

void Sprite::setTexture(const FResourceHandle<Texture> &texture) {
    mTexture = texture;
}

const Rect &Sprite::getBound() const {
    return mBound;
}

void Sprite::setBound(const Rect &bound) {
    mBound = bound;
}
