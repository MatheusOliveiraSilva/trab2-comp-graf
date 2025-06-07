#include <cstdlib>
#include <ctime>
#include "path_tracer.h"
#include "sphere.h"
#include "rectangle.h"
#include "box.h"
#include "rotated_box.h"
#include "material.h"

int main() {
    srand(time(nullptr));

    // Image parameters for higher quality (less noise)
    const int image_width = 600;
    const int image_height = 600;
    const int samples_per_pixel = 300;  // Reduced since cosine sampling is more efficient
    const int max_depth = 8;            // Adjusted depth
    const int min_depth = 1;

    // World
    HittableList world;

    // Materials with adjusted light intensity
    auto red = std::make_shared<Lambertian>(Vec3(0.65, 0.05, 0.05));
    auto white = std::make_shared<Lambertian>(Vec3(0.73, 0.73, 0.73));
    auto green = std::make_shared<Lambertian>(Vec3(0.12, 0.45, 0.15));
    auto light = std::make_shared<DiffuseLight>(Vec3(18, 18, 18));  // Slightly increased

    // Cornell box walls
    world.add(std::make_shared<YZRect>(0, 555, 0, 555, 555, green));  // Left wall
    world.add(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));      // Right wall
    world.add(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));    // Floor
    world.add(std::make_shared<XYRect>(0, 555, 0, 555, 555, white));  // Back wall
    
    // Ceiling with precise hole for light
    world.add(std::make_shared<XZRect>(0, 210, 0, 555, 555, white));     // Left part
    world.add(std::make_shared<XZRect>(346, 555, 0, 555, 555, white));   // Right part  
    world.add(std::make_shared<XZRect>(210, 346, 0, 224, 555, white));   // Front part
    world.add(std::make_shared<XZRect>(210, 346, 335, 555, 555, white)); // Back part
    
    // Light positioned at ceiling level (not below)
    world.add(std::make_shared<DoubleSidedXZRect>(213, 343, 227, 332, 554, light));

    // Rotated boxes for better shadow display
    world.add(std::make_shared<RotatedBox>(Vec3(130, 0, 65), Vec3(295, 165, 230), white, 15));   // Short box rotated 15°
    world.add(std::make_shared<RotatedBox>(Vec3(265, 0, 295), Vec3(430, 330, 460), white, -18)); // Tall box rotated -18°

    // Camera
    Vec3 lookfrom(278, 278, -800);
    Vec3 lookat(278, 278, 0);
    Vec3 vup(0, 1, 0);
    
    Camera cam(lookfrom, lookat, vup, 40.0, (double)image_width / image_height);

    // Render
    render(world, cam, image_width, image_height, samples_per_pixel, max_depth, min_depth);

    return 0;
} 