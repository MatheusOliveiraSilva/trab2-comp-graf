#pragma once
#include "ray.h"
#include <memory>

class Material; // forward declaration

struct HitRecord {
    Vec3 p;
    Vec3 normal;
    std::shared_ptr<Material> mat_ptr;
    double t;
    bool front_face;

    inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual ~Hittable() {}
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
}; 