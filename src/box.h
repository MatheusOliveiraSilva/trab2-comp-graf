#pragma once
#include "hittable.h"
#include "hittable_list.h"
#include "rectangle.h"

class Box : public Hittable {
public:
    Vec3 box_min, box_max;
    HittableList sides;

    Box() {}
    Box(const Vec3& p0, const Vec3& p1, std::shared_ptr<Material> ptr) {
        box_min = p0;
        box_max = p1;

        sides.add(std::make_shared<XYRect>(p0.x, p1.x, p0.y, p1.y, p1.z, ptr));
        sides.add(std::make_shared<XYRect>(p0.x, p1.x, p0.y, p1.y, p0.z, ptr));

        sides.add(std::make_shared<XZRect>(p0.x, p1.x, p0.z, p1.z, p1.y, ptr));
        sides.add(std::make_shared<XZRect>(p0.x, p1.x, p0.z, p1.z, p0.y, ptr));

        sides.add(std::make_shared<YZRect>(p0.y, p1.y, p0.z, p1.z, p1.x, ptr));
        sides.add(std::make_shared<YZRect>(p0.y, p1.y, p0.z, p1.z, p0.x, ptr));
    }

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
        return sides.hit(r, t_min, t_max, rec);
    }
}; 