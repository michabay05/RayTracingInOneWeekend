#include <chrono>
#include <fstream>

#define RAYLIB_VERSION

#ifdef RAYLIB_VERSION
#include "raylib.h"
#endif

#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

rtiw::color RayColor(const rtiw::ray& r, const rtiw::Hittable& world)
{
	rtiw::HitRecord rec;
	if (world.Hit(r, 0, INF, rec)) {
		return 0.5f * (rec.Normal + rtiw::color(1));
	}

	rtiw::vec3 unitDirection = rtiw::Normalize(r.Direction());
	float t = 0.5f * (unitDirection.y() + 1.f);
	return ((1.f - t) * rtiw::color(1.f)) + (t * rtiw::color(0.5f, 0.7f, 1.f));
}

const float ASPECT_RATIO = 16.f / 9.f;
const int IMG_WIDTH = 400;
const int IMG_HEIGHT = (int)(IMG_WIDTH / ASPECT_RATIO);
const int SAMPLES_PER_PIXEL = 100;

#ifdef RAYLIB_VERSION
Texture2D RenderToRaylibTex(rtiw::HittableList& world, rtiw::Camera& cam, Image& image, long long& timeInMS)
{
	auto start = std::chrono::high_resolution_clock::now();
	for (int j = IMG_HEIGHT - 1; j >= 0; j--) {
		for (int i = 0; i < IMG_WIDTH; i++) {
			rtiw::color pixelColor(0);
			for (int s = 0; s < SAMPLES_PER_PIXEL; s++) {
				float u = (i + RandFloat()) / (IMG_WIDTH - 1);
				float v = (j + RandFloat()) / (IMG_HEIGHT - 1);
				rtiw::ray r = cam.GetRay(u, v);
				pixelColor += RayColor(r, world);
			}
			rtiw::color a = UnNormalizeColor(pixelColor, SAMPLES_PER_PIXEL);
			ImageDrawPixel(&image, i, IMG_HEIGHT-j, {(unsigned char)a[0], (unsigned char)a[1], (unsigned char)a[2], 255});
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto durationInMS = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	timeInMS = durationInMS.count();

	return LoadTextureFromImage(image);
}
#else
void RenderToPPM(rtiw::HittableList& world, rtiw::Camera& cam, const std::string filename, long long& timeInMS)
{
	// Open file for writing
	std::ofstream outputFile(filename);
	// Write PPM header
	outputFile << "P3\n" << IMG_WIDTH << ' ' << IMG_HEIGHT << "\n255\n";

	auto start = std::chrono::high_resolution_clock::now();
	for (int j = IMG_HEIGHT - 1; j >= 0; j--) {
		std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < IMG_WIDTH; i++) {
			rtiw::color pixelColor(0);
			for (int s = 0; s < SAMPLES_PER_PIXEL; s++) {
				float u = (i + RandFloat()) / (IMG_WIDTH - 1);
				float v = (j + RandFloat()) / (IMG_HEIGHT - 1);
				rtiw::ray r = cam.GetRay(u, v);
				pixelColor += RayColor(r, world);
			}
			WriteColor(outputFile, pixelColor, SAMPLES_PER_PIXEL);
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto durationInMS = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	timeInMS = durationInMS.count();

	// Close file
	outputFile.close();
}
#endif

int main()
{
	// World
	rtiw::HittableList world;
	world.Add(std::make_shared<rtiw::Sphere>(rtiw::point3::Back(), 0.5f));
	world.Add(std::make_shared<rtiw::Sphere>(rtiw::point3(0, -100.5f, -1.f), 100.f));

	// Camera
	rtiw::Camera cam;

	long long timeInMS = 0;
#ifdef RAYLIB_VERSION
	InitWindow(800, 625, "RayTracing In One Weekend");
	Image img = GenImageColor(IMG_WIDTH, IMG_HEIGHT, BLANK);
	Texture2D tex = RenderToRaylibTex(world, cam, img, timeInMS);

	const Color BACKGROUND{ 20, 20, 20, 255 };
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BACKGROUND);
		DrawTexture(tex, (GetScreenWidth() - tex.width) / 2, (GetScreenHeight() - tex.height) / 2, WHITE);

		DrawText(
			TextFormat("Time to render: %ldms", timeInMS),
			10,
			GetScreenHeight() - 35,
			20,
			RAYWHITE
		);
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