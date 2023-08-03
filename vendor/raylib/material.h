#pragma once

#include "rtweekend.h"

namespace rtiw
{
struct HitRecord;

class Material {
public:
    virtual bool scatter(
        const ray& inRay, const HitRecord& rec, color& attenuation, ray& scattered
    ) const = 0;
};

class Lambertian : public Material
{
public:
    Lambertian(const color& albedo) : m_Albedo(albedo) {}
    virtual bool scatter(
        const ray& inRay, const HitRecord& rec, color& attenuation, ray& scattered
    ) const override {
        vec3 scatterDir = rec.Normal + RandomNormalized();
        // Catch degenerate scatter direction
        if (scatterDir.NearZero())
            scatterDir = rec.Normal;

        scattered = ray(rec.HitPoint, scatterDir);
        attenuation = m_Albedo;
        return true;
    }
private:
    color m_Albedo;
};

class Metal : public Material
{
public:
    Metal(const color& albedo, float fuzz) : m_Albedo(albedo), m_Fuzz(fuzz < 1 ? fuzz : 1) {}
    virtual bool scatter(
        const ray& inRay, const HitRecord& rec, color& attenuation, ray& scattered
    ) const override {
        vec3 reflected = Reflect(Normalize(inRay.Direction()), rec.Normal);
        scattered = ray(rec.HitPoint, reflected + m_Fuzz*RandomInUnitSphere());
        attenuation = m_Albedo;
        return (Dot(scattered.Direction(), rec.Normal) > 0);
    }
private:
    color m_Albedo;
    float m_Fuzz;
};
}