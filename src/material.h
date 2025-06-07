#pragma once
#include "ray.h"
#include "hittable.h"
#include "vec3.h"
#include <memory>

struct ScatterRecord {
    Ray scattered;
    Vec3 attenuation;
    bool is_specular = false;
};

class Material {
public:
    virtual ~Material() {}
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
        return false;
    }
    virtual Vec3 emitted() const { return Vec3(0, 0, 0); }
    virtual bool is_emissive() const { return false; }
};

class Lambertian : public Material {
public:
    Vec3 albedo;
    Lambertian(const Vec3& a) : albedo(a) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override {
        // Use cosine-weighted hemisphere sampling
        Vec3 u, v, w;
        onb_from_w(rec.normal, u, v, w);
        
        Vec3 direction = random_cosine_direction();
        Vec3 scatter_direction = direction.x * u + direction.y * v + direction.z * w;
        
        // Catch degenerate scatter direction
        if (scatter_direction.length_squared() < 1e-8)
            scatter_direction = rec.normal;
            
        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

class DiffuseLight : public Material {
public:
    Vec3 emit_color;
    DiffuseLight(const Vec3& c) : emit_color(c) {}

    virtual Vec3 emitted() const override { return emit_color; }
    virtual bool is_emissive() const override { return true; }
    // Light materials don't scatter - they only emit
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override {
        return false;
    }
}; 