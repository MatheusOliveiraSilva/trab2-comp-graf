#pragma once
#include "hittable_list.h"
#include "camera.h"
#include "material.h"
#include <fstream>
#include <limits>
#include <algorithm>
#include <cmath>
#include <atomic>

// Global runtime flag set by main.cpp to (de)ativar Multiple-Importance Sampling.
extern std::atomic<bool> g_use_mis;

// Sample a point on the rectangular light and return its contribution together with the pdf of the chosen sampling strategy.
struct LightSample {
    Vec3 Li;       // Incoming radiance from the light (already includes geometry term)
    double pdf;    // Pdf value w.r.t. solid angle at the hit point
};

inline LightSample sample_light_direct(const Vec3& hit_point, const Vec3& normal, const Hittable& world) {
    // Light rectangle parameters (matches main.cpp)
    double x0 = 213, x1 = 343, z0 = 227, z1 = 332, y = 554;
    
    // Random point on light
    double u = (double)rand() / RAND_MAX;
    double v = (double)rand() / RAND_MAX;
    Vec3 light_point(x0 + u * (x1 - x0), y, z0 + v * (z1 - z0));
    
    // Direction to light (and related geometric terms)
    Vec3 to_light = light_point - hit_point;
    double distance_squared = to_light.length_squared();
    Vec3 light_dir  = unit_vector(to_light);
    
    // Check if light is visible (shadow ray)
    Ray shadow_ray(hit_point, light_dir);
    HitRecord shadow_rec;
    if (world.hit(shadow_ray, 0.001, std::sqrt(distance_squared) - 0.001, shadow_rec)) {
        if (!shadow_rec.mat_ptr->is_emissive()) {
            return {Vec3(0, 0, 0), 1.0};  // Light is blocked
        }
    }
    
    // Geometry term
    double cos_theta_surface = dot(normal, light_dir);
    if (cos_theta_surface <= 0.0) return {Vec3(0,0,0), 1.0};

    // Light area and emission (must match main.cpp)
    double light_area = (x1 - x0) * (z1 - z0);
    Vec3 light_emission(18, 18, 18);

    // Normal of the light (faces down along -Y)
    Vec3 light_normal(0, -1, 0);
    double cos_theta_light = std::fabs(dot(light_dir * -1.0, light_normal));
    if (cos_theta_light <= 0.0) return {Vec3(0,0,0), 1.0};

    // pdf converting from area measure to solid angle measure
    double pdf_light = distance_squared / (cos_theta_light * light_area);

    Vec3 Li = light_emission * cos_theta_surface / pdf_light;
    return {Li, pdf_light};
}

// Power heuristic for MIS
inline double power_heuristic(double pdf_a, double pdf_b) {
    double a = pdf_a * pdf_a;
    double b = pdf_b * pdf_b;
    return a / (a + b);
}

Vec3 ray_color(const Ray& r, const Hittable& world, int depth, int min_depth) {
    if (depth <= 0)
        return Vec3(0, 0, 0);

    HitRecord rec;
    if (!world.hit(r, 0.001, std::numeric_limits<double>::infinity(), rec)) {
        // background color
        return Vec3(0, 0, 0);
    }

    Vec3 emitted = rec.mat_ptr->emitted();
    
    // If we hit a light source directly, return its emission
    if (rec.mat_ptr->is_emissive()) {
        return emitted;
    }
    
    // For diffuse materials, try to scatter
    Ray scattered;
    Vec3 attenuation;
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
        // RUSSIAN ROULETTE – só começamos depois de cumprir a profundidade mínima
        if (min_depth <= 0) {
            double max_component = std::max(attenuation.x, std::max(attenuation.y, attenuation.z));
            double survival_prob = std::min(max_component, 0.95);  // Cap at 95%
            
            if ((double)rand() / RAND_MAX > survival_prob)
                return emitted; // Terminate
            attenuation = attenuation / survival_prob; // compensate
        }
        
        // === Amostragem de luz direta ===
        LightSample lightSample = sample_light_direct(rec.p, rec.normal, world);

        // pdf da amostragem via BRDF (cosine-weighted)
        double cos_theta = dot(rec.normal, scattered.direction());
        double pdf_brdf = cos_theta > 0.0 ? cos_theta / M_PI : 0.0;

        // Heurísticas de potência (MIS)
        double w_light;
        double w_brdf;
        if (g_use_mis) {
            w_light = power_heuristic(lightSample.pdf, pdf_brdf);
            w_brdf  = power_heuristic(pdf_brdf, lightSample.pdf);
        } else {
            w_light = 0.0;      // sem MIS: consideramos só caminho via BRDF
            w_brdf  = 1.0;
        }

        Vec3 L_direct  = attenuation * w_light * lightSample.Li;

        Vec3 L_indirect = ray_color(scattered, world, depth - 1, min_depth - 1);
        L_indirect = attenuation * w_brdf * L_indirect;

        return emitted + L_direct + L_indirect;
    }
    
    // If material doesn't scatter (like pure emissive), just return emission
    return emitted;
}

void render(const Hittable& world, const Camera& cam, int image_width, int image_height, int samples_per_pixel, int max_depth, int min_depth) {
    std::ofstream out("output.ppm");
    out << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "Scanlines remaining: " << j << '\r';
        for (int i = 0; i < image_width; ++i) {
            Vec3 pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + ((double) rand() / RAND_MAX)) / (image_width - 1);
                auto v = (j + ((double) rand() / RAND_MAX)) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth, min_depth);
            }
            // Write color
            auto scale = 1.0 / samples_per_pixel;
            auto r = pixel_color.x * scale;
            auto g = pixel_color.y * scale;
            auto b = pixel_color.z * scale;
            // Gamma correction sqrt
            r = std::sqrt(r);
            g = std::sqrt(g);
            b = std::sqrt(b);

            int ir = static_cast<int>(256 * std::clamp(r, 0.0, 0.999));
            int ig = static_cast<int>(256 * std::clamp(g, 0.0, 0.999));
            int ib = static_cast<int>(256 * std::clamp(b, 0.0, 0.999));
            out << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    std::cerr << "Done.          \n";
} 