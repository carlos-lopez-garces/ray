#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

vec3 random_in_unit_disk() {
  vec3 p;
  do {
    p = 2.0*vec3(drand48(),drand48(),0) - vec3(1,1,0);

    // The lenght of a vector is the scalar projection onto itself.
    // Keep computing points until one is within a radius of 1 from the origin.
  } while(dot(p,p) >= 1.0);
  return p;
}

class camera {
public:
  // vup is the world's view up vector. vfov is in degrees.
  camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist) {
    lens_radius = aperture/2;

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

    lower_left_corner = origin - half_width*focus_dist*u - half_height*focus_dist*v - focus_dist*w;
    horizontal = 2*half_width*focus_dist*u;
    vertical = 2*half_height*focus_dist*v;
  }

  ray get_ray(float s, float t) {
    vec3 rd = lens_radius*random_in_unit_disk();
    vec3 offset = u * rd.x() + v * rd.y();
    return ray(origin+offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
  }

  vec3 origin;
  vec3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
  vec3 u, v, w;
  float lens_radius;
};

#endif