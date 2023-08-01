#include <chrono>
#include <fstream>

#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

color RayColor(const ray& r, const Hittable& world)
{
    HitRecord rec;
    if (world.Hit(r, 0, INF, rec)) {
        return 0.5f * (rec.Normal + color(1));
    }

    vec3 unitDirection = Normalize(r.Direction());
    float t = 0.5f * (unitDirection.y() + 1.0f);
    return ((1.f - t) * color(1.f)) + (t * color(0.5f, 0.7f, 1.0f));
}

int main()
{
    // Image
    const float ASPECT_RATIO = 16.f / 9.f;
    const int IMG_WIDTH = 400;
    const int IMG_HEIGHT = (int)(IMG_WIDTH / ASPECT_RATIO);
    const int SAMPLES_PER_PIXEL = 100;

    // World
    HittableList world;
    world.Add(std::make_shared<Sphere>(point3::Back(), 0.5f));
    world.Add(std::make_shared<Sphere>(point3(0, -100.5f, -1.f), 100.f));

    // Camera
    Camera cam;

    // Render
    // Open file for writing
    std::ofstream outputFile("output.ppm");
    // Write PPM header
    outputFile << "P3\n" << IMG_WIDTH << ' ' << IMG_HEIGHT << "\n255\n";

    auto start = std::chrono::high_resolution_clock::now();
    for (int j = IMG_HEIGHT - 1; j >= 0; j--) {
        std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < IMG_WIDTH; i++) {
            color pixelColor(0);
            for (int s = 0; s < SAMPLES_PER_PIXEL; s++) {
                float u = (i + RandFloat()) / (IMG_WIDTH - 1);
                float v = (j + RandFloat()) / (IMG_HEIGHT - 1);
                ray r = cam.GetRay(u, v);
                pixelColor += RayColor(r, world);
            }
            WriteColor(outputFile, pixelColor, SAMPLES_PER_PIXEL);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto durationInMS = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "\nDone!\n";
    std::cout << "Took " << durationInMS.count() << "ms to render.\n";

    // Close file
    outputFile.close();

    return 0;
}