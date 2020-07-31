#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "hittable.h"
#include "common.h"

double schlick(double cos_theta, double refractive_idx) {
  auto r0 = (1 - refractive_idx) / (1 + refractive_idx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cos_theta), 5);
}

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

class dielectric : public material {
public:
  dielectric(double refractive_idx) : refractive_idx(refractive_idx) {}

  virtual bool scatter(
    const ray& r, const hit_record& hit, color& attenuation, ray& scattered
  ) const {
    attenuation = color(1.0, 1.0, 1.0);
    const double air_refractive_idx = 1.0;
    
    double eta_over_etap;
    if (hit.front_face) {
      // Ray hits surface from outside, changing medium from air to dielectric.
      eta_over_etap = air_refractive_idx/ this->refractive_idx;
    } else {
      // Ray is exiting the interior of the surface, changing medium from
      // dielectric to air.
      eta_over_etap = this->refractive_idx / air_refractive_idx;
    }

    // Theta is the angle of incidence.
    double cos_theta = fmin(dot(-unit_vector(r.direction()), hit.normal), 1.0);
    // Trigonometric identity.
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    if (eta_over_etap * sin_theta > 1.0) {
      // No solution to Snell's law. Must reflect.
      vec3 reflected = reflect(unit_vector(r.direction()), hit.normal);
      scattered = ray(hit.p, reflected);
      return true;
    }

    double reflect_prob = schlick(cos_theta, eta_over_etap);
    if (random_double() < reflect_prob)
    {
      vec3 reflected = reflect(unit_vector(r.direction()), hit.normal);
      scattered = ray(hit.p, reflected);
      return true;
    }

    vec3 refracted = refract(
      unit_vector(r.direction()), hit.normal, eta_over_etap
    );
    scattered = ray(hit.p, refracted);
    return true;
  }

public:
  double refractive_idx;
};

#endif