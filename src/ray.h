#pragma once

#include "vec3.h"

namespace rtiw
{
class ray
{
  public:
    ray() {}
    ray(const point3& orig, const vec3& dir) : m_Origin(orig), m_Direction(dir) {}

    point3 Origin() const { return m_Origin; }
    vec3 Direction() const { return m_Direction; }

    void SetOrigin(const point3 orig) { m_Origin = orig; }
    void SetDir(const point3 dir) { m_Direction = dir; }

    point3 At(float t) const { return m_Origin + (t * m_Direction); }

  private:
    point3 m_Origin;
    vec3 m_Direction;
};
} // namespace rtiw
