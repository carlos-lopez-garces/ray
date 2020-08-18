#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

vec3 random_in_unit_disk() {
  vec3 p;
  do {
    p = vec3(random_double(-1,1), random_double(-1,1), 0);

    // The square of the length of a vector is the scalar projection onto itself.
    // Keep computing points until one is within a radius of 1 from the origin.
  } while(dot(p,p) >= 1.0);
  return p;
}

class camera {
public:

  // vfov is the vertical field of view.
  camera(
    point3 look_from,
    point3 look_at,
    vec3 vup,
    double vfov,
    double aspect_ratio,
    double aperture,
    double focus_distance
  ) {
    auto theta = degrees_to_radians(vfov);
    // tan is not a linear map, so you can't do just tan(theta).
    auto viewport_height = 2.0 * tan(theta / 2);
    auto viewport_width = aspect_ratio * viewport_height;

    w = unit_vector(look_from - look_at);
    // uv is the image's plane. w is normal to that plane; it is tilted towards the lookat point.
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    origin = look_from;
    horizontal = focus_distance * viewport_width * u;
    vertical = focus_distance * viewport_height * v;

    // Lower left corner of the "film" or image plane.
    lower_left_corner 
      = origin - horizontal / 2 - vertical / 2 - focus_distance * w;
  
    lens_radius = aperture / 2;
  }

  ray get_ray(double s, double t) const {
    vec3 randInDisk = lens_radius * random_in_unit_disk();
    // u and v are the horizontal and vertical vectors of the orthonormal basis of the 
    // camera's frame of orientation.
    // By multiplying the random unit disk point by the orthonormal basis vectors, we 
    // are placing the point on the camera's plane.
    vec3 offset = u * randInDisk.x() + v * randInDisk.y();
    // Note the 2 terms of the 2nd argument: the vector subtraction yields a 
    // direction vector that goes from the camera's position (origin, which is
    // not always the 0 vector) to the "film" or near plane, where the image is.
    return ray(
      // Jittered source.
      origin + offset,
      // s and t are the pixel's real-valued coordinates on the image/film plane.
      // s and t are contained within a single pixel; these coordinates are drawn
      // uniformly at random from within the pixel's boundary. The collection of
      // rays (and the samples retrieved by each one) with randomized destination
      // within the pixel's boundary is what eliminates aliasing.
      //
      // First parenthesis minus origin is the vector that goes from the original,
      // not-jittered origin to the pixel's real-valued coordinate. But since
      // we have jittered the origin, subtracting origin alone does not give you
      // a vector that departs from the jittered origin: the new origin is 
      // origin + offset, or as expressed here, - origin - offset = -(origin + 
      // offset).
      lower_left_corner + s * horizontal + t * vertical - origin - offset
    );
  }

  point3 origin;
  point3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
  // Orthonormal basis for orientation.
  vec3 u, v, w;
  double lens_radius;
};

#endif