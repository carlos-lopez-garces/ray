#include "common.h"

#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <iostream>

point3 sample_unit_sphere(point3 center) {
  vec3 random_direction = vec3::random(-1.0, 1.0);
  return unit_vector(center + random_direction);
}

// ray_color samples the color of a scene using the given ray.
color ray_color(
  const ray &r,
  const hittable &scene,
  const color bg_color_1,
  const color bg_color_2,
  double absorption_factor, 
  int bounces
) {
  if (bounces < 0) {
    return color(0.0, 0.0, 0.0);
  }

  hit_record hit;
  if (scene.hit(r, 0.001, infinity, hit)) {
    color bounce_color;

    point3 unit_sphere_sample_point
      = sample_unit_sphere(hit.p + unit_vector(hit.normal));
    ray bounce_ray(hit.p, unit_sphere_sample_point - hit.p);
    bounce_color = ray_color(
      bounce_ray, scene, bg_color_1, bg_color_2, absorption_factor, bounces - 1
    );

    return absorption_factor * (hit.color + bounce_color);
  }

  // Sample the background, blending blue and white linearly.
  double t = 0.5 + 0.5 * unit_vector(r.direction()).y();
  return t * bg_color_1 + (1 - t) * bg_color_2;
}

int main() {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 384;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 100;
  const int max_bounces = 50;
  const double absorption_factor = 0.5;
  const color bg_color_blue(0.0, 0.0, 0.5);
  const color bg_color_white(1.0, 1.0, 1.0);

  std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

  point3 origin(0.0, 0.0, 0.0);
  vec3 horizontal(4.0, 0.0, 0.0);
  vec3 vertical(0.0, 2.25, 0.0);
  point3 lower_left_corner
    = origin - horizontal / 2 - vertical / 2 - vec3(0.0, 0.0, 1.0);

  // Add a couple of spheres of random color to the scene.
  hittable_list scene;
  scene.add(
    make_shared<sphere>(
      point3(0, 0, -1), 
      0.5, 
      color(random_double(), random_double(), random_double())
    )
  );
  scene.add(
    make_shared<sphere>(
      point3(0, -100.5, -1), 
      100,
      color(random_double(), random_double(), random_double())
    )
  );

  camera cam;

  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
    for (int i = 0; i < image_width; ++i) {
      color pixel_color(0.0, 0.0, 0.0);
      for (int s = 0; s < samples_per_pixel; s++) {
        // Draw from [0, 1). It's important that it not be 1, because we don't
        // want to step on the neighboring pixel.
        auto sample_offset_u = random_double();
        auto sample_offset_v = random_double();
        auto u = (double(i) + sample_offset_u) / (double(image_width) - 1);
        auto v = (double(j) + sample_offset_v) / (double(image_height) - 1);
        ray r = cam.get_ray(u, v);
        pixel_color += ray_color(
          r, scene, bg_color_blue, bg_color_white, 0.5, max_bounces
        );
      }
      write_color(std::cout, pixel_color, samples_per_pixel);
    }
  }

  std::cerr << "\nDone.\n";
}