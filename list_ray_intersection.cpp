#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"

vec3 random_in_unit_sphere() {
  vec3 p;
  do {
    p = 2.0*vec3(drand48(), drand48(), drand48()) - vec3(1,1,1);
  } while (p.squared_length() >= 1.0);
  return p;
}

vec3 color(const ray& r, hitable *world) {
  hit_record rec;
  // t_min=0.0 discards intersection points of objects that are behind the camera.
  // t_min=0.001 also discards reflected rays whose origin is not exactly t=0, but some floating point approximation of it.
  // Those rays cause a problem called shadow acne: The reflected ray intersects its origin surface: This extra bounce will
  // half the origin point's color, causing it to appear darker than the surrounding points.
  if (world->hit(r, 0.001, MAXFLOAT, rec)) {
    vec3 target = rec.p + rec.normal + random_in_unit_sphere();
    // Recursively trace the ray until it doesn't hit anything, halving the color each time it bounces.
    // The color of a hitpoint whose ray bounces too many times tends to black.
    return 0.5*color(ray(rec.p, target-rec.p), world);
  } 
  vec3 unit_direction = unit_vector(r.direction());
  float t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

int main() {
  int nx = 200;
  int ny = 100;
  // Number of antialiasing samples.
  int ns = 100;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";

  hitable *list[2];
  list[0] = new sphere(vec3(0,0,-1), 0.5);
  list[1] = new sphere(vec3(0,-100.5,-1), 100);
  hitable *world = new hitable_list(list, 2);

  camera cam;

  for (int j = ny-1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      vec3 col(0, 0, 0);

      for (int s=0; s < ns; s++) {
        // Pixel sample coordinate.
        float u = float(i + drand48()) / float(nx);
        float v = float(j + drand48()) / float(ny);
        ray r = cam.get_ray(u, v);
        col += color(r, world);
      }

      // Average of pixel color samples.
      col /= float(ns);
      // Gamma correction.
      col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
      int ir = int(255.99*col[0]);
      int ig = int(255.99*col[1]);
      int ib = int(255.99*col[2]);
      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }
}