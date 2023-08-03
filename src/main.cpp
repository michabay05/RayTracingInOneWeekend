#include <chrono>
#include <fstream>

// #define RAYLIB_RENDER

#ifdef RAYLIB_RENDER
#include "raylib.h"
#endif

#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

rtiw::color RayColor(const rtiw::ray& r, const rtiw::Hittable& world, const int depth)
{
    using namespace rtiw;

    if (depth == 0)
        return color(0.f);

    HitRecord rec;
    if (world.Hit(r, 0.0001f, INF, rec)) {
        ray scattered;
        color attenuation;
        if (rec.Mat_Ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * RayColor(scattered, world, depth - 1);
        return color(0.f);
    }

    vec3 unitDirection = Normalize(r.Direction());
    float t = 0.5f * (unitDirection.y() + 1.f);
    return ((1.f - t) * color(1.f)) + (t * color(0.5f, 0.7f, 1.f));
}

const float ASPECT_RATIO = 16.f / 9.f;
const int IMG_WIDTH = 400;
const int IMG_HEIGHT = (int)(IMG_WIDTH / ASPECT_RATIO);
const int SAMPLES_PER_PIXEL = 100;
const int MAX_RAY_BOUNCES = 50;

#ifdef RAYLIB_RENDER
Texture2D RenderToRaylibTex(rtiw::HittableList& world, rtiw::Camera& cam, Image& image,
                            long long& timeInMS)
#else
void RenderToPPM(rtiw::HittableList& world, rtiw::Camera& cam, const std::string filename,
                 long long& timeInMS)
#endif
{
#ifndef RAYLIB_RENDER
    // Open file for writing
    std::ofstream outputFile(filename);
    // Write PPM header
    outputFile << "P3\n" << IMG_WIDTH << ' ' << IMG_HEIGHT << "\n255\n";
#endif

    auto start = std::chrono::high_resolution_clock::now();
    for (int j = IMG_HEIGHT - 1; j >= 0; j--) {
#ifndef RAYLIB_RENDER
        std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;
#endif
        for (int i = 0; i < IMG_WIDTH; i++) {
            rtiw::color pixelColor(0);
            for (int s = 0; s < SAMPLES_PER_PIXEL; s++) {
                float u = (i + RandFloat()) / (IMG_WIDTH - 1);
                float v = (j + RandFloat()) / (IMG_HEIGHT - 1);
                rtiw::ray r = cam.GetRay(u, v);
                pixelColor += RayColor(r, world, MAX_RAY_BOUNCES);
            }
#ifdef RAYLIB_RENDER
            rtiw::color a = UnNormalizeColor(pixelColor, SAMPLES_PER_PIXEL);
            ImageDrawPixel(&image, i, IMG_HEIGHT - j,
                           {(unsigned char)a[0], (unsigned char)a[1], (unsigned char)a[2], 255});
#else
            WriteColor(outputFile, pixelColor, SAMPLES_PER_PIXEL);
#endif
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto durationInMS = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    timeInMS = durationInMS.count();

#ifdef RAYLIB_RENDER
    return LoadTextureFromImage(image);
#else
    // Close file
    outputFile.close();
#endif
}

int main()
{
    // World
    rtiw::HittableList world;

    auto materialGround = std::make_shared<rtiw::Lambertian>(rtiw::color(0.8f, 0.8f, 0.f));
    auto materialCenter = std::make_shared<rtiw::Lambertian>(rtiw::color(0.7f, 0.3f, 0.3f));
    auto materialLeft = std::make_shared<rtiw::Metal>(rtiw::color(0.8f), 0.3f);
    auto materialRight = std::make_shared<rtiw::Metal>(rtiw::color(0.8f, 0.6f, 0.2f), 1.f);

    world.Add(
        std::make_shared<rtiw::Sphere>(rtiw::point3(0.f, -100.5f, -1.0f), 100.f, materialGround));
    world.Add(std::make_shared<rtiw::Sphere>(rtiw::point3(0.f, 0.f, -1.0f), 0.5f, materialCenter));
    world.Add(std::make_shared<rtiw::Sphere>(rtiw::point3(-1.f, 0.f, -1.0f), 0.5f, materialLeft));
    world.Add(std::make_shared<rtiw::Sphere>(rtiw::point3(1.f, 0.f, -1.0f), 0.5f, materialRight));

    // Camera
    rtiw::Camera cam;

    long long timeInMS = 0;
#ifdef RAYLIB_RENDER
    InitWindow(800, 625, "RayTracing In One Weekend");
    Image img = GenImageColor(IMG_WIDTH, IMG_HEIGHT, BLANK);
    Texture2D tex = RenderToRaylibTex(world, cam, img, timeInMS);

    const Color BACKGROUND{20, 20, 20, 255};
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BACKGROUND);
        DrawTexture(tex, (GetScreenWidth() - tex.width) / 2, (GetScreenHeight() - tex.height) / 2,
                    WHITE);

        DrawText(TextFormat("Time to render: %ldms", timeInMS), 10, GetScreenHeight() - 35, 20,
                 RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
#else
    RenderToPPM(world, cam, "output.ppm", timeInMS);

    std::cout << "\nDone!\n";
    std::cout << "Took " << timeInMS << "ms to render.\n";
#endif

    return 0;
}