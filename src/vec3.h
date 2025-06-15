#pragma once
#include <cmath>
#include <iostream>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Vec3 {
public:
    double x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

    Vec3 operator-() const { return Vec3(-x, -y, -z); }

    Vec3& operator+=(const Vec3& v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }

    Vec3& operator*=(const double t) {
        x *= t; y *= t; z *= t;
        return *this;
    }

    Vec3& operator/=(const double t) {
        return *this *= 1 / t;
    }

    double length() const { return std::sqrt(length_squared()); }
    double length_squared() const { return x * x + y * y + z * z; }

    inline static Vec3 random() {
        return Vec3(random_double(), random_double(), random_double());
    }

    inline static Vec3 random(double min, double max) {
        return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

private:
    static double random_double() {
        return rand() / (RAND_MAX + 1.0);
    }
    static double random_double(double min, double max) {
        return min + (max - min) * random_double();
    }
};

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline Vec3 operator*(double t, const Vec3& v) {
    return Vec3(t * v.x, t * v.y, t * v.z);
}

inline Vec3 operator*(const Vec3& v, double t) {
    return t * v;
}

inline Vec3 operator/(Vec3 v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vec3& u, const Vec3& v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
    return Vec3(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}

inline Vec3 unit_vector(Vec3 v) {
    return v / v.length();
}

inline Vec3 random_in_unit_sphere() {
    while (true) {
        Vec3 p = Vec3::random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

inline Vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
} 

// Cosine-weighted hemisphere sampling for better diffuse lighting
inline Vec3 random_cosine_direction() {
    auto r1 = (double) rand() / RAND_MAX;
    auto r2 = (double) rand() / RAND_MAX;
    auto z = std::sqrt(1 - r2);
    
    auto phi = 2 * M_PI * r1;
    auto x = std::cos(phi) * std::sqrt(r2);
    auto y = std::sin(phi) * std::sqrt(r2);
    
    return Vec3(x, y, z);
}

// Helper to create orthonormal basis from a normal
inline void onb_from_w(const Vec3& n, Vec3& u, Vec3& v, Vec3& w) {
    w = unit_vector(n);
    Vec3 a = (std::abs(w.x) > 0.9) ? Vec3(0, 1, 0) : Vec3(1, 0, 0);
    v = unit_vector(cross(w, a));
    u = cross(w, v);
} 