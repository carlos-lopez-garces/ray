#include <iostream>
#include "ray.h"

// hit_sphere returns the value of the parameter t (the smallest one if there are 2 roots) 
// if the ray intersects a sphere centered at center and of the given radius.
float hit_sphere(const vec3& center, float radius, const ray& r) {
  vec3 oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = 2.0 * dot(oc, r.direction());
  float c = dot(oc, oc) - radius*radius;
  float discriminant = b*b - 4*a*c;
  // 0, 1, 2 roots.
  if (discriminant < 0) {
    // No hit.
    return -1.0;
  }
  // Quadratic formula.
  return (-b - sqrt(discriminant)) / (2*a);
}

// color returns the color of the pixel intersected by this ray.
// The scene contains a sphere centered at (0,0,-1) and of radius 0.5.
// The background is a lerp of white and blue.
// The normals of the sphere are visualized using a color map.
vec3 color(const ray& r) {
  float t = hit_sphere(vec3(0, 0, -1), 0.5, r);
  if (t > 0.0) {
    // P-C, where P is the hitpoint and C is the center of the sphere.
    vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0,0,-1));
    // Map unit vector component value [-1,1] to [0,1].
    return 0.5 * vec3(N.x()+1, N.y()+1, N.z()+1);
  }
  vec3 unit_direction = unit_vector(r.direction());
  t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

int main() {
  int nx = 200;
  int ny = 100;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";
  
  vec3 lower_left_corner(-2.0, -1.0, -1.0);
  vec3 horizontal(4.0, 0.0, 0.0);
  vec3 vertical(0.0, 2.0, 0.0);
  vec3 origin(0.0, 0.0, 0.0);

  for (int j = ny-1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      // Pixel coordinate.
      float u = float(i) / float(nx);
      float v = float(j) / float(ny);

      ray r(origin, lower_left_corner + u*horizontal + v*vertical);

      vec3 col = color(r);
      int ir = int(255.99*col[0]);
      int ig = int(255.99*col[1]);
      int ib = int(255.99*col[2]);
      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }
}