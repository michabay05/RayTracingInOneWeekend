#pragma once

#include "rtweekend.h"

class Camera
{
public:
    Camera()
    {
        const float ASPECT_RATIO = 16.f / 9.f;
        float viewportHeight = 2.f;
        float viewportWidth = ASPECT_RATIO * viewportHeight;
        float focalLength = 1.f;

        m_Origin = point3::Zero();
        m_Horizontal = vec3(viewportWidth, 0.f, 0.f);
        m_Vertical = vec3(0.f, viewportHeight, 0.f);
        m_LowerLeftCorner = m_Origin - (m_Horizontal / 2) - (m_Vertical / 2) - vec3(0, 0, focalLength);
    }

    ray GetRay(float u, float v) const
    {
        return ray(m_Origin, m_LowerLeftCorner + u * m_Horizontal + v * m_Vertical - m_Origin);
    }

private:
    point3 m_Origin;
    point3 m_LowerLeftCorner;
    vec3 m_Horizontal;
    vec3 m_Vertical;
};