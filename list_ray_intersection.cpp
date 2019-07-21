#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"

vec3 color(const ray& r, hitable *world) {
  hit_record rec;
  // t_min=0.0 discards intersection points of objects that are behind the camera.
  if (world->hit(r, 0.0, MAXFLOAT, rec)) {
    // Map unit vector component value [-1,1] to [0,1].
    return 0.5*vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
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
      int ir = int(255.99*col[0]);
      int ig = int(255.99*col[1]);
      int ib = int(255.99*col[2]);
      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }
}