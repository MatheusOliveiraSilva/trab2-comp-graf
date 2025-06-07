#include <cstdlib>
#include <ctime>
#include "path_tracer.h"
#include "sphere.h"
#include "rectangle.h"
#include "box.h"
#include "transform.h"
#include "material.h"

int main() {
    srand(time(nullptr));

    // Image
    const int image_width = 600;
    const int image_height = 600;
    const int samples_per_pixel = 64;
    const int max_depth = 4;
    const int min_depth = 3;

    // World
    HittableList world;

    // Materials
    auto red = std::make_shared<Lambertian>(Vec3(0.65, 0.05, 0.05));
    auto white = std::make_shared<Lambertian>(Vec3(0.73, 0.73, 0.73));
    auto green = std::make_shared<Lambertian>(Vec3(0.12, 0.45, 0.15));
    auto light = std::make_shared<DiffuseLight>(Vec3(15, 15, 15));

    // Cornell box walls
    world.add(std::make_shared<YZRect>(0, 555, 0, 555, 555, green));  // Left wall
    world.add(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));      // Right wall
    world.add(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));    // Floor
    world.add(std::make_shared<XYRect>(0, 555, 0, 555, 555, white));  // Back wall
    
    // Ceiling with hole for light (making sure light area is completely open)
    world.add(std::make_shared<XZRect>(0, 210, 0, 555, 555, white));      // Left part of ceiling
    world.add(std::make_shared<XZRect>(350, 555, 0, 555, 555, white));    // Right part of ceiling
    world.add(std::make_shared<XZRect>(210, 350, 0, 220, 555, white));    // Front part of ceiling
    world.add(std::make_shared<XZRect>(210, 350, 340, 555, 555, white));  // Back part of ceiling
    
    // Light (positioned well below ceiling to ensure visibility)
    world.add(std::make_shared<XZRect>(210, 350, 220, 340, 550, light));
    
    // Rotated boxes for better shadow visualization
    // Box 1 (tall box) - positioned on the left side, rotated 15 degrees
    auto box1 = std::make_shared<Box>(Vec3(0, 0, 0), Vec3(165, 330, 165), white);
    auto rotated_box1 = std::make_shared<RotateY>(box1, 15);
    auto positioned_box1 = std::make_shared<Translate>(rotated_box1, Vec3(265, 0, 295));
    world.add(positioned_box1);
    
    // Box 2 (short box) - positioned on the right side, rotated -18 degrees  
    auto box2 = std::make_shared<Box>(Vec3(0, 0, 0), Vec3(165, 165, 165), white);
    auto rotated_box2 = std::make_shared<RotateY>(box2, -18);
    auto positioned_box2 = std::make_shared<Translate>(rotated_box2, Vec3(130, 0, 65));
    world.add(positioned_box2);

    // Camera
    Vec3 lookfrom(278, 278, -800);
    Vec3 lookat(278, 278, 0);
    Vec3 vup(0, 1, 0);
    
    Camera cam(lookfrom, lookat, vup, 40.0, (double)image_width / image_height);

    // Render
    render(world, cam, image_width, image_height, samples_per_pixel, max_depth, min_depth);

    return 0;
} 