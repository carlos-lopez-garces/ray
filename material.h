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

class metal : public material {
public:
  metal(color albedo) : albedo(albedo) {}

  virtual bool scatter(
    const ray& r, const hit_record& hit, color& attenuation, ray& scattered
  ) const {
    vec3 reflected 
      = reflect(unit_vector(r.direction()), hit.normal);
    scattered = ray(hit.p, reflected);
    attenuation = this->albedo;
    return dot(scattered.direction(), hit.normal) > 0;
  }

  color albedo;
};

class fuzzy : public material {
public:
  fuzzy(color albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz: 1) {}

  virtual bool scatter(
    const ray& r, const hit_record& hit, color& attenuation, ray& scattered
  ) const {
    vec3 reflected
      = reflect(unit_vector(r.direction()), hit.normal);
    // The fuzz increases the radius of the sampled sphere.
    // The bigger the sphere, the fuzzier the reflection.
    vec3 fuzzed = reflected + fuzz*this->sample_unit_sphere(vec3(0.0, 0.0, 0.0));
    scattered = ray(hit.p, fuzzed);
    attenuation = this->albedo;
    return dot(scattered.direction(), hit.normal) > 0;
  }

private:
  point3 sample_unit_sphere(point3 center) const {
    while (true) {
      // Sample an axis-aligned unit cube; reject the sample if it is outside the
      // unit sphere.
      vec3 random_direction = vec3::random(-1.0, 1.0);
      if (random_direction.length_squared() > 1.0) continue;
      return unit_vector(center + random_direction);
    }
  }

public:
  color albedo;
  double fuzz;
};

#endif