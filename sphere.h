#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
  sphere() : radius(0.0), exterior_color(0, 0, 0), interior_color(0, 0, 0) {};
  sphere(
    point3 center,
    double radius,
    color exterior_color,
    color interior_color
  )
    : center(center),
      radius(radius),
      exterior_color(exterior_color),
      interior_color(interior_color) {};

  virtual bool 
    hit(const ray& r, double t_min, double t_max, hit_record& rec) const;

  point3 center;
  double radius;
  color exterior_color;
  color interior_color;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec)
  const {
  vec3 oc = r.origin() - center;
  auto a = r.direction().length_squared();
  auto half_b = dot(oc, r.direction());
  auto c = oc.length_squared() - radius * radius;
  auto discriminant = half_b * half_b - a * c;

  if (discriminant > 0) {
    auto root = sqrt(discriminant);
    // One root.
    auto temp = (-half_b - root) / a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.at(rec.t);
      rec.color = exterior_color;
      vec3 outward_normal = (rec.p - center) / radius;
      // The outward_normal always points away from the surface. But the hit's
      // normal depends on whether it is on the front or back face of the
      // surface.
      rec.set_face_normal(r, outward_normal);
      return true;
    }
    // The other root.
    temp = (-half_b + root) / a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.at(rec.t);
      rec.color = interior_color;
      vec3 outward_normal = (rec.p - center) / radius;
      rec.set_face_normal(r, outward_normal);
      return true;
    }
  }

  return false;
}

#endif