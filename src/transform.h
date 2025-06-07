#pragma once
#include "vec3.h"
#include "ray.h"
#include <cmath>

class Transform {
public:
    static Vec3 rotate_y(const Vec3& p, double angle) {
        double cos_theta = cos(angle);
        double sin_theta = sin(angle);
        return Vec3(cos_theta * p.x + sin_theta * p.z,
                   p.y,
                   -sin_theta * p.x + cos_theta * p.z);
    }
    
    static Ray rotate_ray_y(const Ray& r, double angle) {
        return Ray(rotate_y(r.origin(), -angle), rotate_y(r.direction(), -angle));
    }
    
    static Vec3 rotate_normal_y(const Vec3& n, double angle) {
        return rotate_y(n, angle);
    }
}; 