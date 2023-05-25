#include "Glyph.h"

bool FGlyph::operator<(const FGlyph &g) const {
    if (ch < g.ch) {
        return true;
    } else if (ch > g.ch) {
        return false;
    }

    return size < g.size;
}

bool FGlyph::operator==(const FGlyph &rhs) const {
    return ch == rhs.ch &&
           size == rhs.size &&
           bold == rhs.bold &&
           italic == rhs.italic;
}

bool FGlyph::operator!=(const FGlyph &rhs) const {
    return !(rhs == *this);
}
