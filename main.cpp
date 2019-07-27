#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"

vec3 color(const ray& r, hitable *world, int depth) {
  hit_record rec;
  // t_min=0.0 discards intersection points of objects that are behind the camera.
  // t_min=0.001 also discards reflected rays whose origin is not exactly t=0, but some floating point approximation of it.
  // Those rays cause a problem called shadow acne: The reflected ray intersects its origin surface: This extra bounce will
  // half the origin point's color, causing it to appear darker than the surrounding points.
  if (world->hit(r, 0.001, MAXFLOAT, rec)) {
    ray scattered;
    vec3 attenuation;
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      // Recursively trace the ray until it doesn't hit anything, attenuating the color each time it bounces.
      // The color of a hitpoint whose ray bounces too many times tends to black.
      return attenuation * color(scattered, world, depth+1);
    }
    return vec3(0,0,0);
  } 
  vec3 unit_direction = unit_vector(r.direction());
  float t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

hitable *random_scene() {
  int n = 500;
  hitable **list = new hitable*[n+1];
  list[0] = new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(0.5,0.5,0.5)));
  
  int i = 1;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float choose_mat = drand48();
      vec3 center(a+0.9*drand48(), 0.2, b+0.9*drand48());
      if ((center-vec3(4,0.2,0)).length() > 0.9) {
        if (choose_mat < 0.8) {
          // Diffuse.
          list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand48()*drand48(),drand48()*drand48(),drand48()*drand48())));
        } else if (choose_mat < 0.95) {
          // Metal.
          list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1+drand48()), 0.5*(1+drand48()), 0.5*(1+drand48())), 0.5*drand48()));
        } else {
          // Glass.
          list[i++] = new sphere(center, 0.2, new dielectric(1.5));
        }
      }
    }
  }

  list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
  list[i++] = new sphere(vec3(-4, 1,0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
  list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
  
  return new hitable_list(list, i);
}

int main() {
  int nx = 1280;
  int ny = 720;
  // Number of antialiasing samples.
  int ns = 100;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";

  hitable *world = random_scene();

  vec3 lookfrom(1,1,2);
  vec3 lookat(0,0,-1);
  float dist_to_focus = (lookfrom-lookat).length();
  float aperture = 0.0;
  camera cam(lookfrom, lookat, vec3(0,1,0), 90, float(nx)/float(ny), aperture, dist_to_focus);

  for (int j = ny-1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      vec3 col(0, 0, 0);

      for (int s=0; s < ns; s++) {
        // Pixel sample coordinate.
        float u = float(i + drand48()) / float(nx);
        float v = float(j + drand48()) / float(ny);
        ray r = cam.get_ray(u, v);
        col += color(r, world, 0);
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