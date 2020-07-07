#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "hittable.h"

class material {
public:
  virtual bool scatter(
    const ray &r, const hit_record &hit, color &attenuation, ray &scattered
  ) const = 0;
};

#endif