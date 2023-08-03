#pragma once

#include <cmath>
#include <iostream>

#include "rtweekend.h"

namespace rtiw
{
class vec3
{
  public:
    vec3() : m_El{0, 0, 0} {}
    vec3(float val) : m_El{val, val, val} {}
    vec3(float e0, float e1, float e2) : m_El{e0, e1, e2} {}

    float x() const { return m_El[0]; }
    float y() const { return m_El[1]; }
    float z() const { return m_El[2]; }

    vec3 operator-() const { return vec3(-m_El[0], -m_El[1], -m_El[2]); }
    float operator[](int i) const { return m_El[i]; }
    float& operator[](int i) { return m_El[i]; }

    inline static vec3 Back() { return vec3(0.f, 0.f, -1.f); }
    inline static vec3 Zero() { return vec3(0.f); }

    inline static vec3 Random(float min, float max)
    {
        return vec3(RandFloat(min, max), RandFloat(min, max), RandFloat(min, max));
    }
    inline static vec3 Random() { return vec3(RandFloat(), RandFloat(), RandFloat()); }

    inline bool NearZero() const {
        // Return true if the vector is close to zero in all dimensions
        const float eps = (float)0.000001;
        return (fabs(m_El[0]) < eps) && (fabs(m_El[1]) < eps) && (fabs(m_El[2]) < eps);
    }

    vec3& operator+=(const vec3& v)
    {
        m_El[0] += v.m_El[0];
        m_El[1] += v.m_El[1];
        m_El[2] += v.m_El[2];

        return *this;
    }

    vec3& operator*=(const float t)
    {
        m_El[0] *= t;
        m_El[1] *= t;
        m_El[2] *= t;

        return *this;
    }

    vec3& operator/=(const float t) { return *this *= 1 / t; }

    float Length() const { return sqrtf(LengthSquared()); }

    float LengthSquared() const
    {
        return (m_El[0] * m_El[0]) + (m_El[1] * m_El[1]) + (m_El[2] * m_El[2]);
    }

  private:
    float m_El[3];
};

// Type aliases
using point3 = vec3; // 3D point
using color = vec3;  // RGB color

inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
    return (out << v[0] << ' ' << v[1] << ' ' << v[2]);
}

inline vec3 operator+(const vec3& u, const vec3& v)
{
    return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline vec3 operator+(const vec3& u, const float t) { return vec3(u[0] + t, u[1] + t, u[2] + t); }

inline vec3 operator-(const vec3& u, const vec3& v)
{
    return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v)
{
    return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline vec3 operator*(float t, const vec3& v) { return vec3(t * v[0], t * v[1], t * v[2]); }

inline vec3 operator*(const vec3& v, float t) { return t * v; }

inline vec3 operator/(const vec3& v, float t) { return (1 / t) * v; }

inline float Dot(const vec3& u, const vec3& v) { return u[0] * v[0] + u[1] * v[1] + u[2] * v[2]; }

inline vec3 Cross(const vec3& u, const vec3& v)
{
    return vec3(u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]);
}

inline vec3 Normalize(const vec3& v) { return v / v.Length(); }

inline static vec3 RandomInUnitSphere()
{
    while (true) {
        auto p = vec3::Random(-1, 1);
        if (p.LengthSquared() >= 1)
            continue;
        return p;
    }
}

inline vec3 RandomNormalized() { return Normalize(RandomInUnitSphere()); }

inline vec3 RandomInHemisphere(const vec3& normal) {
    vec3 inUnitSphere = RandomInUnitSphere();
    if (Dot(inUnitSphere, normal) > 0.f) // In the same hemisphere as the normal
        return inUnitSphere;
    else
        return -inUnitSphere;
}

inline vec3 Reflect(const vec3& v, const vec3& n) {
    return v - (2 * Dot(v, n) * n);
}
} // namespace rtiw
