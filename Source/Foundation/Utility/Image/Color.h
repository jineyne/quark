#pragma once

#include "UtilityCore.h"

class DLL_EXPORT FColor {
public:
    // Static default colors
    static const FColor Black;
    static const FColor White;
    static const FColor Red;
    static const FColor Green;
    static const FColor Blue;

private:
    float red;
    float green;
    float blue;
    float alpha;

public:
    // Constructors
    FColor();
    FColor(float r, float g, float b, float a = 1.0);

public:
    bool operator==(const FColor &rhs) const;
    bool operator!=(const FColor &rhs) const;

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
    struct hash<FColor> {
        size_t operator()(const FColor &color) const {
            size_t hash = 0;
            CombineHash(hash, color.getRed());
            CombineHash(hash, color.getGreen());
            CombineHash(hash, color.getBlue());
            CombineHash(hash, color.getAlpha());

            return hash;
        }
    };
}