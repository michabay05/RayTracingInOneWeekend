#pragma once

#include "vec3.h"

#include <iostream>

inline void WriteColor(std::ostream& out, color pixelColor, int samplesPerPixel)
{
    float r = pixelColor.x();
    float g = pixelColor.y();
    float b = pixelColor.z();

    // Divide the color by the number of samples
    float scale = 1.f / samplesPerPixel;
    r *= scale;
    g *= scale;
    b *= scale;

    out << (int)(256 * Clamp(r, 0.f, 0.999f)) << ' '
        << (int)(256 * Clamp(g, 0.f, 0.999f)) << ' '
        << (int)(256 * Clamp(b, 0.f, 0.999f)) << '\n';
}
