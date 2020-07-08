#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "hittable.h"
#include "common.h"

class material {
public:
  virtual bool scatter(
    const ray &r, const hit_record &hit, color &attenuation, ray &scattered
  ) const = 0;
};

class lambertian : public material {
public:
  lambertian(const color &albedo) : albedo(albedo) {}

  virtual bool scatter(
    const ray& r, const hit_record& hit, color& attenuation, ray& scattered
  ) const {
    vec3 scatter_direction = hit.normal + unit_vector(vec3::random());
    scattered = ray(hit.p, scatter_direction);
    attenuation = this->albedo;
    return true;
  }

  color albedo;
};

#endif