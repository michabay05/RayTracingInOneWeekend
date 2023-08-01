#pragma once

#include "vec3.h"

class ray
{
public:
    ray() {}
    ray(const point3& orig, const vec3& dir)
        : origin(orig), direction(dir) {}

    point3 Origin() const { return origin; }
    vec3 Direction() const { return direction; }

    void SetOrigin(const point3 orig) { origin = orig; }
    void SetDir(const point3 dir) { direction = dir; }

    point3 At(float t) const { return origin + (t * direction); }

private:
    point3 origin;
    vec3 direction;
};
