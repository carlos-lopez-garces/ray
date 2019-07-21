#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere: public hitable {
public:
  sphere() {}

  sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat(m) {};

  virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

  vec3 center;
  float radius;
  material *mat;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
  vec3 oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = dot(oc, r.direction());
  float c = dot(oc, oc) - radius*radius;
  // 0, 1, 2 roots.
  float discriminant = b*b - a*c;
  if (discriminant > 0) {
    // Quadratic formula. Root of closest hit.
    float temp = (-b - sqrt(discriminant))/a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      // Normalized.
      rec.normal = (rec.p - center) / radius;
      rec.mat_ptr = mat;
      return true;
    }
    // Root of farthest hit.
    temp = (-b + sqrt(discriminant))/a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      // Normalized.
      rec.normal = (rec.p - center) / radius;
      rec.mat_ptr = mat;
      return true;
    }
  }
  return false;
}

#endif