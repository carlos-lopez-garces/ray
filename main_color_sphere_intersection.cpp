#include "ray.h"
#include "color.h"
#include "vec3.h"

#include <iostream>

// hit_sphere determine whether the ray intersects the given sphere.
bool hit_sphere(const point3& center, double radius, const ray& r) {
  // We could find the values of t for which the ray `origin + t*direction`
  // yields a point on the sphere. Such values are the real roots of the
  // quadratic equation ax^2 + bx + c = 0, where a, b, and c are:
  double a = dot(r.direction(), r.direction());
  double b = dot(2 * r.direction(), r.origin() - center);
  double c = dot(r.origin() - center, r.origin() - center) - (radius * radius);
  // (The unit vector of the origin can't be computed, because it would require
  // a division by 0, its length.)

  // But the discriminant alone will tell us the number of real roots:
  double discriminant = (b * b) - (4 * a * c);

  // If it's <0, there are no real roots and the ray doesn't intersect the
  // sphere. If it's 0, there is only 1 real root and the ray intersects the
  // sphere tangentially at 1 point. If it's >0, there are 2 real roots and the
  // ray intersects the sphere at 2 points (it passes through it).
  return discriminant >= 0;
}

// ray_color samples the color of a scene using the given ray. The scene
// contains only a red sphere.
color ray_color(const ray& r) {
  // The ray's origin, the camera, is placed at (0, 0, 0). The sphere is
  // directly in front of it.
  point3 sphere_center = point3(0, 0, -1);
  double sphere_radius = 0.5;
  if (hit_sphere(sphere_center, sphere_radius, r)) {
    return color(1, 0, 0);
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