#include "Color.h"

// Static default colors initialization
const FColor FColor::Black(0, 0, 0);
const FColor FColor::White(255, 255, 255);
const FColor FColor::Red(255, 0, 0);
const FColor FColor::Green(0, 255, 0);
const FColor FColor::Blue(0, 0, 255);

FColor::FColor() : red(0), green(0), blue(0), alpha(255) {}

FColor::FColor(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
        : red(r), green(g), blue(b), alpha(a) {}

bool FColor::operator==(const FColor &rhs) const {
    return red == rhs.red && green == rhs.green && blue == rhs.blue && alpha == rhs.alpha;
}

bool FColor::operator!=(const FColor &rhs) const {
    return !(rhs == *this);
}

uint32_t FColor::operator*() const {
    return toUnsignedInt();
}

uint32_t FColor::getBrightness() const {
    return (red + green + blue) / 3;
}

uint32_t FColor::getGray() const {
    return 0.3 * red + 0.59 * green + 0.11 * blue;
}

uint32_t FColor::toUnsignedInt() const {
    return (red << 24) | (green << 16) | (blue << 8) | alpha;
}
