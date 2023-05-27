#include "Color.h"

// Static default colors initialization
const Color Color::Black(0, 0, 0);
const Color Color::White(1, 1, 1);
const Color Color::Red(1, 0, 0);
const Color Color::Green(0, 1, 0);
const Color Color::Blue(0, 0, 1);

Color::Color() : red(0), green(0), blue(0), alpha(255) {}

Color::Color(float r, float g, float b, float a)
        : red(r), green(g), blue(b), alpha(a) {}

Color Color::FromRGBA(uint32_t rgba) {
    Color output;
    const uint32_t val32 = rgba;

    output.setAlpha(((val32 >> 24) & 0xFF) / 255.0f);
    output.setBlue(((val32 >> 16) & 0xFF) / 255.0f);
    output.setGreen(((val32 >> 8) & 0xFF) / 255.0f);
    output.setRed((val32 & 0xFF) / 255.0f);

    return output;
}

Color Color::FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    Color output;

    output.setAlpha(a / 255.0f);
    output.setBlue(b / 255.0f);
    output.setGreen(g / 255.0f);
    output.setRed(r / 255.0f);

    return output;
}

bool Color::operator==(const Color &rhs) const {
    return red == rhs.red && green == rhs.green && blue == rhs.blue && alpha == rhs.alpha;
}

bool Color::operator!=(const Color &rhs) const {
    return !(rhs == *this);
}

float Color::operator*() const {
    return toUnsignedInt();
}

float Color::getBrightness() const {
    return (red + green + blue) / 3;
}

float Color::getGray() const {
    return 0.3 * red + 0.59 * green + 0.11 * blue;
}

float Color::toUnsignedInt() const {
    return (((int) (red * 255)) << 24) | (((int) (green * 255)) << 16) | (((int) (blue * 255)) << 8) | ((int) (alpha * 255));
}
