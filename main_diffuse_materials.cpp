#include "common.h"

#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <iostream>

point3 sample_unit_cube(point3 center) {
  vec3 random_direction = vec3::random(-1.0, 1.0);
  return unit_vector(center + random_direction);
}

point3 sample_unit_sphere(point3 center) {
  while (true) {
    // Sample an axis-aligned unit cube; reject the sample if it is outside the
    // unit sphere.
    vec3 random_direction = vec3::random(-1.0, 1.0);
    if (random_direction.length_squared() > 1.0) continue;
    return unit_vector(center + random_direction);
  }
}

point3 sample_cylinder(point3 center) {
  auto a = random_double(0, 2*pi);
  auto z = random_double(-1, 1);
  auto r = sqrt(1 - z*z);
  return center + vec3(r*cos(a), r*sin(a), z);
}

// ray_color samples the color of a scene using the given ray.
color ray_color(
  const ray &r,
  const hittable &scene,
  const color bg_color_1,
  const color bg_color_2,
  double absorption_factor, 
  int bounces,
  bool use_hit_color
) {
  if (bounces < 0) {
    return color(0.0, 0.0, 0.0);
  }

  hit_record hit;
  // t_min is 0.001, instead of 0.0, to avoid shadow acne caused by the bouncing
  // ray hitting its origin surface.
  if (scene.hit(r, 0.001, infinity, hit)) {
    color bounce_color;

    // Sampling a cylinder produces a more uniform scattering, whereas sampling
    // a unit sphere scatters with a bias towards the normal.
    point3 cylinder_sample_point = sample_cylinder(hit.p + hit.normal);

    ray bounce_ray(hit.p, cylinder_sample_point - hit.p);
    bounce_color = ray_color(
      bounce_ray,
      scene,
      bg_color_1,
      bg_color_2,
      absorption_factor,
      bounces - 1,
      use_hit_color
    );

    color sample_color = absorption_factor * bounce_color;
    if (use_hit_color) {
      sample_color += absorption_factor * hit.color;
    }
    return sample_color;
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
  const color bg_color_1(0.5, 0.7, 1.0);
  const color bg_color_2(1.0, 1.0, 1.0);

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
      color(157.0/256.0, 163.0/256.0, 154.0/256.0),
      color(219.0/256.0, 41.0/256.0, 85.0/256.0)
    )
  );
  scene.add(
    make_shared<sphere>(
      point3(0, -100.5, -1), 
      100,
      color(random_double(), random_double(), random_double()),
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
          r, scene, bg_color_1, bg_color_2, 0.5, max_bounces, true
        );
      }
      write_color(std::cout, pixel_color, samples_per_pixel);
    }
  }

  std::cerr << "\nDone.\n";
}