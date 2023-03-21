#include "Color.h"

// Static default colors initialization
const FColor FColor::Black(0, 0, 0);
const FColor FColor::White(1, 1, 1);
const FColor FColor::Red(1, 0, 0);
const FColor FColor::Green(0, 1, 0);
const FColor FColor::Blue(0, 0, 1);

FColor::FColor() : red(0), green(0), blue(0), alpha(255) {}

FColor::FColor(float r, float g, float b, float a)
        : red(r), green(g), blue(b), alpha(a) {}

bool FColor::operator==(const FColor &rhs) const {
    return red == rhs.red && green == rhs.green && blue == rhs.blue && alpha == rhs.alpha;
}

bool FColor::operator!=(const FColor &rhs) const {
    return !(rhs == *this);
}

float FColor::operator*() const {
    return toUnsignedInt();
}

float FColor::getBrightness() const {
    return (red + green + blue) / 3;
}

float FColor::getGray() const {
    return 0.3 * red + 0.59 * green + 0.11 * blue;
}

float FColor::toUnsignedInt() const {
    return (((int) (red * 255)) << 24) | (((int) (green * 255)) << 16) | (((int) (blue * 255)) << 8) | ((int) (alpha * 255));
}
