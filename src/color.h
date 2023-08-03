#pragma once

#include "vec3.h"

#include <iostream>

inline rtiw::color UnNormalizeColor(rtiw::color pixelColor, int samplesPerPixel)
{
    float r = pixelColor.x();
    float g = pixelColor.y();
    float b = pixelColor.z();

    // Divide the color by the number of samples
    float scale = 1.f / samplesPerPixel;
    r = sqrtf(scale * r);
    g = sqrtf(scale * g);
    b = sqrtf(scale * b);

    return rtiw::color((int)(256 * Clamp(r, 0.f, 0.999f)), (int)(256 * Clamp(g, 0.f, 0.999f)),
                       (int)(256 * Clamp(b, 0.f, 0.999f)));
}

inline void WriteColor(std::ostream& out, rtiw::color pixelColor, int samplesPerPixel)
{
    rtiw::color c = UnNormalizeColor(pixelColor, samplesPerPixel);

    out << c << "\n";
}
