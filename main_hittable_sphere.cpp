#include "ray.h"
#include "color.h"
#include "vec3.h"
#include "sphere.h"

#include <iostream>

// ray_color samples the color of a scene using the given ray. The scene
// contains a sphere that is shaded with the values of its surface normals.
color ray_color(const ray& r) {
  // The ray's origin, the camera, is placed at (0, 0, 0). The sphere is
  // directly in front of it.
  point3 sphere_center = point3(0, 0, -1);
  double sphere_radius = 0.5;

  sphere s(sphere_center, sphere_radius);

  hit_record hit;
  if (s.hit(r, 0.0, 100.0, hit)) {
    color gray(0.5, 0.5, 0.5);
    return (color)hit.normal + gray;
  }

  // Sample the background, blending blue and white linearly.
  double t = 0.5 + 0.5 * unit_vector(r.direction()).y();
  return t * color(0, 0, 0.5) + (1 - t) * color(1, 1, 1);
}

int main() {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 384;
  const int image_height = static_cast<int>(image_width / aspect_ratio);

  std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

  point3 origin(0.0, 0.0, 0.0);
  vec3 horizontal(4.0, 0.0, 0.0);
  vec3 vertical(0.0, 2.25, 0.0);
  point3 lower_left_corner
    = origin - horizontal / 2 - vertical / 2 - vec3(0.0, 0.0, 1.0);

  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
    for (int i = 0; i < image_width; ++i) {
      auto u = double(i) / (double(image_width) - 1);
      auto v = double(j) / (double(image_height) - 1);
      ray r(origin, lower_left_corner + u * horizontal + v * vertical);
      color pixel_color = ray_color(r);
      write_color(std::cout, pixel_color);
    }
  }

  std::cerr << "\nDone.\n";
}