#pragma once

#include "hittable.h"

namespace rtiw
{
class Sphere : public Hittable
{
  public:
    Sphere() {}
    Sphere(point3 c, float r, std::shared_ptr<Material> m) : m_Center(c), m_Radius(r), m_Mat_Ptr(m)
    {
    }

    virtual bool Hit(const ray& r, float tMin, float tMax, HitRecord& rec) const override;

  private:
    point3 m_Center;
    float m_Radius;
    std::shared_ptr<Material> m_Mat_Ptr;
};

bool Sphere::Hit(const ray& r, float tMin, float tMax, HitRecord& rec) const
{
    vec3 oc = r.Origin() - m_Center;
    float a = r.Direction().LengthSquared();
    float half_b = Dot(oc, r.Direction());
    float c = oc.LengthSquared() - (m_Radius * m_Radius);
    // Quadratic formula discriminant
    float discriminant = (half_b * half_b) - (a * c);
    if (discriminant < 0)
        return false;

    // Find the nearest root that lies in the acceptable range
    float sqrtDiscriminant = sqrtf(discriminant);
    float root = (-half_b - sqrtDiscriminant) / a;
    if (root < tMin || root > tMax) {
        root = (-half_b + sqrtDiscriminant) / a;
        if (root < tMin || root > tMax) {
            // Solution is outside the range (tMin, tMax)
            return false;
        }
    }

    rec.t = root;
    rec.HitPoint = r.At(rec.t);
    vec3 outwardNormal = (rec.HitPoint - m_Center) / m_Radius;
    rec.SetFaceNormal(r, outwardNormal);
    rec.Mat_Ptr = m_Mat_Ptr;

    return true;
}
} // namespace rtiw
