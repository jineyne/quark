#pragma once

#include "UtilityCore.h"
#include "Color.g.h"

QSTRUCT()
struct DLL_EXPORT Color {
    GENERATED_BODY()

public:
    // Static default colors
    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;

private:
    QPROPERTY()
    float red;

    QPROPERTY()
    float green;

    QPROPERTY()
    float blue;

    QPROPERTY()
    float alpha;

public:
    // Constructors
    Color();
    Color(float r, float g, float b, float a = 1.0);

public:
    static Color FromRGBA(uint32_t rgba);
    static Color FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

public:
    bool operator==(const Color &rhs) const;
    bool operator!=(const Color &rhs) const;

    float operator *() const;

public:
    // Accessors
    float getRed() const { return red; }
    float getGreen() const { return green; }
    float getBlue() const { return blue; }
    float getAlpha() const { return alpha; }

    // Mutators
    void setRed(float r) { red = r; }
    void setGreen(float g) { green = g; }
    void setBlue(float b) { blue = b; }
    void setAlpha(float a) { alpha = a; }

    // Calculation functions
    float getBrightness() const;

    float getGray() const;

    // Conversion functions
    float toUnsignedInt() const;
};

namespace std {
    template<>
    struct hash<Color> {
        size_t operator()(const Color &color) const {
            size_t hash = 0;
            CombineHash(hash, color.getRed());
            CombineHash(hash, color.getGreen());
            CombineHash(hash, color.getBlue());
            CombineHash(hash, color.getAlpha());

            return hash;
        }
    };
}