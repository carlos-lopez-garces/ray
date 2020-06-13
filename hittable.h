#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct hit_record {
  point3 p;
  vec3 normal;
  double t;
  color color;

  // front_face tells if the surface was hit on its front face / exterior.
  bool front_face;

  inline void set_face_normal(const ray& r, const vec3& outward_normal) {
    // Ray and normal go in the same direction about a line tangent to the
    // surface.
    front_face = dot(r.direction(), outward_normal) < 0.0;
    // When the surface was hit on its back face, the normal points inward.
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class hittable {
public:
  virtual bool 
    hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;
};

#endif