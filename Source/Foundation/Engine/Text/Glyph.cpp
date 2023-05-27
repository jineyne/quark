#include "Glyph.h"

bool Glyph::operator<(const Glyph &g) const {
    if (ch < g.ch) {
        return true;
    } else if (ch > g.ch) {
        return false;
    }

    return size < g.size;
}

bool Glyph::operator==(const Glyph &rhs) const {
    return ch == rhs.ch &&
           size == rhs.size &&
           bold == rhs.bold &&
           italic == rhs.italic;
}

bool Glyph::operator!=(const Glyph &rhs) const {
    return !(rhs == *this);
}
