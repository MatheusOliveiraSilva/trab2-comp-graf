#pragma once
#include "hittable_list.h"
#include "camera.h"
#include "material.h"
#include <fstream>
#include <limits>
#include <algorithm>

Vec3 ray_color(const Ray& r, const Hittable& world, int depth, int min_depth) {
    if (depth <= 0)
        return Vec3(0, 0, 0);

    HitRecord rec;
    if (!world.hit(r, 0.001, std::numeric_limits<double>::infinity(), rec)) {
        // background color
        return Vec3(0, 0, 0);
    }

    Ray scattered;
    Vec3 attenuation;
    Vec3 emitted = rec.mat_ptr->emitted();

    // If we hit emissive material and have satisfied minimum depth
    if (rec.mat_ptr->is_emissive() && min_depth <= 0) {
        return emitted;
    }

    // Try to scatter
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
        return attenuation * ray_color(scattered, world, depth - 1, min_depth - 1);
    }

    // If we can't scatter and haven't satisfied minimum depth, return black
    return Vec3(0, 0, 0);
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