#pragma once
#include "hittable.h"
#include "material.h"

class XYRect : public Hittable {
public:
    double x0, x1, y0, y1, k;
    std::shared_ptr<Material> mp;

    XYRect() {}
    XYRect(double _x0, double _x1, double _y0, double _y1, double _k,
           std::shared_ptr<Material> mat)
        : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {}

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
        auto t = (k - r.origin().z) / r.direction().z;
        if (t < t_min || t > t_max)
            return false;
        auto x = r.origin().x + t * r.direction().x;
        auto y = r.origin().y + t * r.direction().y;
        if (x < x0 || x > x1 || y < y0 || y > y1)
            return false;
        rec.t = t;
        rec.p = r.at(t);
        Vec3 outward_normal = Vec3(0, 0, 1);
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mp;
        return true;
    }
};

class XZRect : public Hittable {
public:
    double x0, x1, z0, z1, k;
    std::shared_ptr<Material> mp;

    XZRect() {}
    XZRect(double _x0, double _x1, double _z0, double _z1, double _k,
           std::shared_ptr<Material> mat)
        : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {}

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
        auto t = (k - r.origin().y) / r.direction().y;
        if (t < t_min || t > t_max)
            return false;
        auto x = r.origin().x + t * r.direction().x;
        auto z = r.origin().z + t * r.direction().z;
        if (x < x0 || x > x1 || z < z0 || z > z1)
            return false;
        rec.t = t;
        rec.p = r.at(t);
        Vec3 outward_normal = Vec3(0, 1, 0);
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mp;
        return true;
    }
};

class YZRect : public Hittable {
public:
    double y0, y1, z0, z1, k;
    std::shared_ptr<Material> mp;

    YZRect() {}
    YZRect(double _y0, double _y1, double _z0, double _z1, double _k,
           std::shared_ptr<Material> mat)
        : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {}

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
        auto t = (k - r.origin().x) / r.direction().x;
        if (t < t_min || t > t_max)
            return false;
        auto y = r.origin().y + t * r.direction().y;
        auto z = r.origin().z + t * r.direction().z;
        if (y < y0 || y > y1 || z < z0 || z > z1)
            return false;
        rec.t = t;
        rec.p = r.at(t);
        Vec3 outward_normal = Vec3(1, 0, 0);
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mp;
        return true;
    }
}; 