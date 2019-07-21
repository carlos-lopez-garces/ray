#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera {
public:
  // vup is the world's view up vector. vfov is in degrees.
  camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect) {
    vec3 u, v, w;

    // Convert to radians.
    float theta = vfov*M_PI/180;
    float half_height = tan(theta/2);
    float half_width = aspect * half_height;

    origin = lookfrom;
    // w points from the scene towards the camera.
    w = unit_vector(lookfrom-lookat);
    // uv is the image's plane. w is normal to that plane; it is tilted towards the lookat point.
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    lower_left_corner = vec3(-half_width, -half_height, -1.0);
    lower_left_corner = origin - half_width*u - half_height*v - w;
    horizontal = 2*half_width*u;
    vertical = 2*half_height*v;
  }

  ray get_ray(float u, float v) {
    return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
  }

  vec3 origin;
  vec3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
};

#endif