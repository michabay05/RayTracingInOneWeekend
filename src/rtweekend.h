#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

/* ----------------------- Constants ----------------------- */
const float INF = std::numeric_limits<float>::infinity();
const float PI = (float)3.1415926535897932385;

/* ----------------------- Utility functions ----------------------- */
inline float DegreesToRadians(float degrees) {
    return degrees * PI / 180.f;
}

// Returns a random real number in [0, 1)
inline float RandFloat() {
    return (float)rand() / (RAND_MAX + 1.f);
}

// Returns a random real number in [min, max)
inline float RandFloat(float min, float max) {
    return min + (max - min) * RandFloat();
}

inline float Clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

/* ----------------------- Common headers ----------------------- */
#include "ray.h"
#include "vec3.h"