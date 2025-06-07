#pragma once
#include "hittable.h"
#include "box.h"
#include "transform.h"

class RotatedBox : public Hittable {
public:
    std::shared_ptr<Box> box;
    double sin_theta;
    double cos_theta;
    Vec3 center;

    RotatedBox(const Vec3& p0, const Vec3& p1, std::shared_ptr<Material> ptr, double angle) {
        box = std::make_shared<Box>(p0, p1, ptr);
        auto radians = angle * M_PI / 180.0;
        sin_theta = sin(radians);
        cos_theta = cos(radians);
        center = (p0 + p1) * 0.5;
    }

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
        // Transform ray to object space
        Vec3 origin = r.origin() - center;
        Vec3 direction = r.direction();
        
        // Rotate ray backwards
        Vec3 rotated_origin = Vec3(
            cos_theta * origin.x - sin_theta * origin.z,
            origin.y,
            sin_theta * origin.x + cos_theta * origin.z
        ) + center;
        
        Vec3 rotated_direction = Vec3(
            cos_theta * direction.x - sin_theta * direction.z,
            direction.y,
            sin_theta * direction.x + cos_theta * direction.z
        );
        
        Ray rotated_ray(rotated_origin, rotated_direction);
        
        if (!box->hit(rotated_ray, t_min, t_max, rec))
            return false;
        
        // Transform hit point and normal back to world space
        Vec3 p = rec.p - center;
        rec.p = Vec3(
            cos_theta * p.x + sin_theta * p.z,
            p.y,
            -sin_theta * p.x + cos_theta * p.z
        ) + center;
        
        Vec3 n = rec.normal;
        rec.normal = Vec3(
            cos_theta * n.x + sin_theta * n.z,
            n.y,
            -sin_theta * n.x + cos_theta * n.z
        );
        
        return true;
    }
}; 