#pragma once

#include "ray.h"

namespace rtiw
{
struct HitRecord
{
    point3 HitPoint;
    vec3 Normal;
    float t;
    bool FrontFace;

    inline void SetFaceNormal(const ray& r, const vec3& outwardNormal)
    {
        FrontFace = Dot(r.Direction(), outwardNormal) < 0;
        Normal = FrontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable
{
  public:
    virtual bool Hit(const ray& r, float tMin, float tMax, HitRecord& rec) const = 0;
};
} // namespace rtiw