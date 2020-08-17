#include "common.h"

#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>

// ray_color samples the color of a scene using the given ray.
color ray_color(
  const ray& r,
  const hittable& scene,
  const color bg_color_1,
  const color bg_color_2,
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
    color attenuation;

    ray bounce_ray;
    if (!hit.material->scatter(r, hit, attenuation, bounce_ray)) {
      return color(0.0, 0.0, 0.0);
    }

    bounce_color = ray_color(
      bounce_ray,
      scene,
      bg_color_1,
      bg_color_2,
      bounces - 1,
      use_hit_color
    );

    color sample_color = attenuation * bounce_color;
    if (use_hit_color) {
      sample_color += attenuation * hit.color;
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
  const color bg_color_1(134.0 / 256.0, 154.0 / 256.0, 181.0 / 256.0);
  const color bg_color_2(134.0 / 256.0, 154.0 / 256.0, 181.0 / 256.0);
  const double glass_refractive_index = 1.5;

  std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

  point3 origin(0.0, 0.0, 0.0);
  vec3 horizontal(4.0, 0.0, 0.0);
  vec3 vertical(0.0, 2.25, 0.0);
  point3 lower_left_corner
    = origin - horizontal / 2 - vertical / 2 - vec3(0.0, 0.0, 1.0);

  hittable_list scene;

  shared_ptr<fuzzy> mat1 = make_shared<fuzzy>(color(0.0, 0.0, 0.0), 0.5);
  scene.add(
    make_shared<sphere>(
      point3(1.0, 0, -1),
      0.5,
      color(177.0 / 256.0, 169.0 / 256.0, 107.0 / 256.0),
      color(177.0 / 256.0, 169.0 / 256.0, 107.0 / 256.0),
      mat1
      )
  );

  shared_ptr<lambertian> mat2 = make_shared<lambertian>(color(0.1, 0.2, 0.5));
  scene.add(
    make_shared<sphere>(
      point3(0.0, 0, -1),
      0.5,
      color(171.0 / 256.0, 117.0 / 256.0, 133.0 / 256.0),
      color(171.0 / 256.0, 117.0 / 256.0, 133.0 / 256.0),
      mat2
      )
  );

  shared_ptr<dielectric> mat3 = make_shared<dielectric>(glass_refractive_index);
  scene.add(
    make_shared<sphere>(
      point3(-1.0, 0, -1),
      0.5,
      color(0.0 / 256.0, 0.0 / 256.0, 0.0 / 256.0),
      color(0.0 / 256.0, 0.0 / 256.0, 0.0 / 256.0),
      mat3
      )
  );
  scene.add(
    make_shared<sphere>(
      point3(-1.0, 0, -1),
      // The negative radius doesn't affect the geometry, because the equation
      // of the sphere squares the radius. The normals get inverted, though,
      // which is just what we need for the inner walls of a hollow glass ball.
      -0.4,
      color(0.0 / 256.0, 0.0 / 256.0, 0.0 / 256.0),
      color(0.0 / 256.0, 0.0 / 256.0, 0.0 / 256.0),
      mat3
      )
  );

  // Ground.
  shared_ptr<lambertian> mat4 = make_shared<lambertian>(color(134.0 / 256.0, 154.0 / 256.0, 181.0 / 256.0));
  scene.add(
    make_shared<sphere>(
      point3(0, -100.5, -1),
      100,
      color(134.0 / 256.0, 154.0 / 256.0, 181.0 / 256.0),
      color(134.0 / 256.0, 154.0 / 256.0, 181.0 / 256.0),
      mat4
      )
  );

  camera cam(point3(-2, 2, 1), point3(0, 0, -1), vec3(0, 1, 0), 20, aspect_ratio);

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
          r, scene, bg_color_1, bg_color_2, max_bounces, false
        );
      }
      write_color(std::cout, pixel_color, samples_per_pixel);
    }
  }

  std::cerr << "\nDone.\n";
}