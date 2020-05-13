#include "ray.h"
#include "color.h"
#include "vec3.h"

#include <iostream>

// ray_color fakes the sample taken by a ray. instead of returning the color
// of a view sample of a scene, it returns a color that interpolates linearly
// between white and sky blue.
color ray_color(const ray& r) {
  vec3 unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 384;
  const int image_height = static_cast<int>(image_width / aspect_ratio);

  std::cout << "p3\n" << image_width << " " << image_height << "\n255\n";

  point3 origin(0.0, 0.0, 0.0);
  vec3 horizontal(4.0, 0.0, 0.0);
  vec3 vertical(0.0, 2.25, 0.0);
  point3 lower_left_corner
    = origin - horizontal / 2 - vertical / 2 - vec3(0.0, 0.0, 1.0);

  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rscanlines remaining: " << j << " " << std::flush;
    for (int i = 0; i < image_width; ++i) {
      auto u = double(i) / (double(image_width) - 1);
      auto v = double(j) / (double(image_height) - 1);
      ray r(origin, lower_left_corner + u * horizontal + v * vertical);
      color pixel_color = ray_color(r);
      write_color(std::cout, pixel_color);
    }
  }

  std::cerr << "\ndone.\n";
}