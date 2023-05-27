#include "TextureView.h"

size_t TextureView::HashFunction::operator()(const TextureViewDesc &key) const {
    size_t seed = 0;
    CombineHash(seed, key.mostDetailMip);
    CombineHash(seed, key.mipCount);
    CombineHash(seed, key.firstArraySlice);
    CombineHash(seed, key.arraySlicesCount);
    CombineHash(seed, key.usage);
    return seed;
}

bool TextureView::EqualFunction::operator()(const TextureViewDesc &a, const TextureViewDesc &b) const {
    return a.mostDetailMip == b.mostDetailMip && a.mipCount == b.mipCount &&
           a.firstArraySlice == b.firstArraySlice && a.arraySlicesCount == b.arraySlicesCount && a.usage == b.usage;
}

TextureView::TextureView(const TextureViewDesc &desc) : mDesc(desc) {}