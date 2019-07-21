#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

struct hit_record {
  float t;
  vec3 p;
  vec3 normal;
};

class hitable {
public:
  // [t_min,t_max] is the range of values of t of valid hits.
  virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif