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

  camera() {
    auto aspect_ratio = 16.0 / 9.0;
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    origin = point3(0, 0, 0);
    horizontal = vec3(viewport_width, 0.0, 0.0);
    vertical = vec3(0.0, viewport_height, 0.0);

    // Lower left corner of the "film" or image plane.
    lower_left_corner 
      = origin - horizontal / 2 - vertical / 2 - vec3(0.0, 0.0, focal_length);
  }

  //// vup is the world's view up vector. vfov is in degrees.
  //camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist) {
  //  lens_radius = aperture/2;

  //  // Convert to radians.
  //  float theta = vfov*pi/180;
  //  float half_height = tan(theta/2);
  //  float half_width = aspect * half_height;

  //  origin = lookfrom;
  //  // w points from the scene towards the camera.
  //  w = unit_vector(lookfrom-lookat);
  //  // uv is the image's plane. w is normal to that plane; it is tilted towards the lookat point.
  //  u = unit_vector(cross(vup, w));
  //  v = cross(w, u);

  //  lower_left_corner = origin - half_width*focus_dist*u - half_height*focus_dist*v - focus_dist*w;
  //  horizontal = 2*half_width*focus_dist*u;
  //  vertical = 2*half_height*focus_dist*v;
  //}

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