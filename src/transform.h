#pragma once
#include "hittable.h"
#include "vec3.h"

class RotateY : public Hittable {
public:
    std::shared_ptr<Hittable> ptr;
    double sin_theta;
    double cos_theta;

    RotateY(std::shared_ptr<Hittable> p, double angle) : ptr(p) {
        auto radians = angle * M_PI / 180;
        sin_theta = sin(radians);
        cos_theta = cos(radians);
    }

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
        Vec3 origin = r.origin();
        Vec3 direction = r.direction();

        origin.x = cos_theta * r.origin().x - sin_theta * r.origin().z;
        origin.z = sin_theta * r.origin().x + cos_theta * r.origin().z;

        direction.x = cos_theta * r.direction().x - sin_theta * r.direction().z;
        direction.z = sin_theta * r.direction().x + cos_theta * r.direction().z;

        Ray rotated_r(origin, direction);

        if (!ptr->hit(rotated_r, t_min, t_max, rec))
            return false;

        Vec3 p = rec.p;
        Vec3 normal = rec.normal;

        p.x = cos_theta * rec.p.x + sin_theta * rec.p.z;
        p.z = -sin_theta * rec.p.x + cos_theta * rec.p.z;

        normal.x = cos_theta * rec.normal.x + sin_theta * rec.normal.z;
        normal.z = -sin_theta * rec.normal.x + cos_theta * rec.normal.z;

        rec.p = p;
        rec.set_face_normal(rotated_r, normal);

        return true;
    }
};

class Translate : public Hittable {
public:
    std::shared_ptr<Hittable> ptr;
    Vec3 offset;

    Translate(std::shared_ptr<Hittable> p, const Vec3& displacement)
        : ptr(p), offset(displacement) {}

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
        Ray moved_r(r.origin() - offset, r.direction());
        if (!ptr->hit(moved_r, t_min, t_max, rec))
            return false;

        rec.p += offset;
        rec.set_face_normal(moved_r, rec.normal);

        return true;
    }
}; 