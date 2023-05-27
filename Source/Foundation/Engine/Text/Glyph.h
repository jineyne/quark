#pragma once

#include "CorePrerequisites.h"
#include "Math/Size.h"
#include "Math/Vector2.h"

struct GlyphData {
    float u = 0, v = 0, w = 0, h = 0;
    int32_t left = 0, top = 0;
    Size bitmap = Size::ZeroSize;
    Vector2 advance;
    int32_t height = 0;
    int32_t use = 0;
};

struct DLL_EXPORT Glyph {
    TCHAR ch;
    uint32_t size;
    bool bold = false;
    bool italic = false;


    bool operator<(const Glyph &g) const;

    bool operator==(const Glyph &rhs) const;
    bool operator!=(const Glyph &rhs) const;
};

template<>
struct std::hash<Glyph> {
    size_t operator()(const Glyph &glyph) const {
        size_t result = 0;
        CombineHash(result, glyph.ch);
        CombineHash(result, glyph.size);
        CombineHash(result, glyph.bold);
        CombineHash(result, glyph.italic);
        return result;
    }
};