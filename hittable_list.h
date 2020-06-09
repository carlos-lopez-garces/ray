#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list: public hittable {
public:
  hittable_list() {}
  
  hittable_list(shared_ptr<hittable> object) { 
    add(object);
  }

  void clear() {
    objects.clear();
  }

  void add(shared_ptr<hittable> object) {
    objects.push_back(object);
  }

  virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const;

  std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
  hit_record temp_rec;
  bool hit_anything = false;
  auto closest_so_far = t_max;

  for (const auto &object : objects) {
    // closest_so_far is t_max so that if the ray hits an object that is farther
    // away, the hit gets ignored.
    if (object->hit(r, t_min, closest_so_far, temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  return hit_anything;
}

#endif