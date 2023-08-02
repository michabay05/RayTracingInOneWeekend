#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

namespace rtiw
{
class HittableList : public Hittable
{
  public:
    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object) { Add(object); }

    void Clear() { m_Objects.clear(); }
    void Add(std::shared_ptr<Hittable> object) { m_Objects.push_back(object); }

    virtual bool Hit(const ray& r, float tMin, float tMax, HitRecord& rec) const override;

  private:
    std::vector<std::shared_ptr<Hittable>> m_Objects;
};

bool HittableList::Hit(const ray& r, float tMin, float tMax, HitRecord& rec) const
{
    HitRecord tempRecord;
    bool hitAnything = false;
    float closestSoFar = tMax;

    for (const auto& object : m_Objects) {
        if (object->Hit(r, tMin, closestSoFar, tempRecord)) {
            hitAnything = true;
            closestSoFar = tempRecord.t;
            rec = tempRecord;
        }
    }

    return hitAnything;
}
} // namespace rtiw