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
    uint32_t red;
    uint32_t green;
    uint32_t blue;
    uint32_t alpha;

public:
    // Constructors
    FColor();
    FColor(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 255);

public:
    bool operator==(const FColor &rhs) const;
    bool operator!=(const FColor &rhs) const;

    uint32_t operator *() const;

public:
    // Accessors
    uint32_t getRed() const { return red; }
    uint32_t getGreen() const { return green; }
    uint32_t getBlue() const { return blue; }
    uint32_t getAlpha() const { return alpha; }

    // Mutators
    void setRed(uint32_t r) { red = r; }
    void setGreen(uint32_t g) { green = g; }
    void setBlue(uint32_t b) { blue = b; }
    void setAlpha(uint32_t a) { alpha = a; }

    // Calculation functions
    uint32_t getBrightness() const;

    uint32_t getGray() const;

    // Conversion functions
    uint32_t toUnsignedInt() const;
};
