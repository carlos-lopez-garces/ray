#include "ray.h"
#include "color.h"
#include "vec3.h"

#include <iostream>

// hits determines if the input ray intersects the input sphere and where. It
// returns the number of hits (0, 1 if tangential, or 2), and if there are any,
// the hits IN/OUT argument will return the points.
int hits(
  const point3& center,
  double radius,
  const ray& r,
  point3* hits) {

  double a = dot(r.direction(), r.direction());
  double b = dot(2 * r.direction(), r.origin() - center);
  double c = dot(r.origin() - center, r.origin() - center) - (radius * radius);

  double discriminant = (b * b) - (4 * a * c);

  if (discriminant < 0) {
    return 0;
  }

  double divisor = 2 * dot(r.direction(), r.direction());

  double t = -2 * dot(r.direction(), r.origin() - center) / divisor;

  double tFirstHit = t + discriminant / divisor;

  hits[0] = r.at(t);

  if (discriminant == 0) {
    return 1;
  }

  double tSecondHit = t - discriminant / divisor;

  hits[1] = r.at(t);

  return 2;
}

// sphere_normal_at_point returns the normal of a sphere centered at the given
// center and at the given surface point.
point3 sphere_normal_at_point(
  const point3& center,
  const point3& surface_point) {

  return surface_point - center;
}

// ray_color samples the color of a scene using the given ray. The scene
// contains a sphere that is shaded with the values of its surface normals.
color ray_color(const ray& r) {
  // The ray's origin, the camera, is placed at (0, 0, 0). The sphere is
  // directly in front of it.
  point3 sphere_center = point3(0, 0, -1);
  double sphere_radius = 0.5;

  point3 hitPoints[2];
  int hitCount = hits(sphere_center, sphere_radius, r, hitPoints);

  color gray(0.5, 0.5, 0.5);
  if (hitCount == 1) {
    return (color)
      sphere_normal_at_point(sphere_center, hitPoints[0])
      + gray;
  }
  else if (hitCount == 2) {
    // Find the nearest point to the camera.
    const double distanceToFirstHit = (hitPoints[0] - r.origin()).length();
    const double distanceToSecondHit = (hitPoints[1] - r.origin()).length();
    if (distanceToFirstHit <= distanceToSecondHit) {
      return (color)
        sphere_normal_at_point(sphere_center, hitPoints[0])
        + gray;
    }
    else {
      return (color)
        sphere_normal_at_point(sphere_center, hitPoints[1])
        + gray;
    }
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