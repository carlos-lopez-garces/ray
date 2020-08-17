#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

vec3 random_in_unit_disk() {
  vec3 p;
  do {
    p = 2.0*vec3(random_double(), random_double(), 0) - vec3(1,1,0);

    // The length of a vector is the scalar projection onto itself.
    // Keep computing points until one is within a radius of 1 from the origin.
  } while(dot(p,p) >= 1.0);
  return p;
}

class camera {
public:

  // vfov is the vertical field of view.
  camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio) {
    auto theta = degrees_to_radians(vfov);
    // tan is not a linear map, so you can't do just tan(theta).
    auto viewport_height = 2.0 * tan(theta / 2);
    auto viewport_width = aspect_ratio * viewport_height;

    auto w = unit_vector(lookfrom - lookat);
    // uv is the image's plane. w is normal to that plane; it is tilted towards the lookat point.
    auto u = unit_vector(cross(vup, w));
    auto v = cross(w, u);

    origin = lookfrom;
    horizontal = viewport_width * u;
    vertical = viewport_height * v;

    // Lower left corner of the "film" or image plane.
    lower_left_corner 
      = origin - horizontal / 2 - vertical / 2 - w;
  }

  ray get_ray(double u, double v) const {
    // Note the 2 terms of the 2nd argument: the vector subtraction yields a 
    // direction vector that goes from the camera's position (origin, which is
    // not always the 0 vector) to the "film" or near plane, where the image is.
    return ray(origin, (lower_left_corner + u*horizontal + v*vertical) - origin);
  }

  point3 origin;
  point3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
};

#endif