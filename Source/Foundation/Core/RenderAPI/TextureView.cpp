#include "TextureView.h"

size_t FTextureView::HashFunction::operator()(const FTextureViewDesc &key) const {
    size_t seed = 0;
    CombineHash(seed, key.mostDetailMip);
    CombineHash(seed, key.mipCount);
    CombineHash(seed, key.firstArraySlice);
    CombineHash(seed, key.arraySlicesCount);
    CombineHash(seed, key.usage);
    return seed;
}

bool FTextureView::EqualFunction::operator()(const FTextureViewDesc &a, const FTextureViewDesc &b) const {
    return a.mostDetailMip == b.mostDetailMip && a.mipCount == b.mipCount &&
           a.firstArraySlice == b.firstArraySlice && a.arraySlicesCount == b.arraySlicesCount && a.usage == b.usage;
}

FTextureView::FTextureView(const FTextureViewDesc &desc) : mDesc(desc) {}