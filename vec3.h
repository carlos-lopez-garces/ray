#ifndef VEC3_H
#define VEC3_H

#include "common.h"

#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3 {
public:
  vec3() : e{0, 0, 0} {}
  
  vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

  inline static vec3 random() {
    return vec3(random_double(), random_double(), random_double());
  }

  inline static vec3 random(double min, double max) {
    return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
  }

  double x() const { return e[0]; }
  double y() const { return e[1]; }
  double z() const { return e[2]; }
  double r() const { return e[0]; }
  double g() const { return e[1]; }
  double b() const { return e[2]; }

  vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
  double operator[](int i) const { return e[i]; }
  double& operator[](int i) { return e[i]; }

  vec3& operator+=(const vec3 &v);
  vec3& operator*=(const double t);
  vec3& operator/=(const double t);

  double length() const {
    return sqrt(length_squared());
  }

  double length_squared() const {
    return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
  }

  double e[3];
};

vec3& vec3::operator+=(const vec3& v) {
  e[0] += v.e[0];
  e[1] += v.e[1];
  e[2] += v.e[2];
  return *this;
}

vec3& vec3::operator*=(const double t) {
  e[0] *= t;
  e[1] *= t;
  e[2] *= t;
  return *this;
}

vec3& vec3::operator/=(const double t) {
  return *this *= 1 / t;
}

// Aliases.
using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
  return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
  return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator/(const vec3 &v, double t) {
  return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline vec3 operator*(const vec3 &v, double t) {
  return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

inline double dot(const vec3 &u, const vec3 &v) {
  return u.e[0]*v.e[0] + u.e[1]*v.e[1] + u.e[2]*v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
  return vec3(
    u.e[1]*v.e[2] - u.e[2]*v.e[1],
    -(u.e[0]*v.e[2] - u.e[2]*v.e[0]),
    u.e[0]*v.e[1] - u.e[1]*v.e[0]
  );
}

inline vec3 reflect(const vec3& v, const vec3& normal) {
  return v - 2 * dot(v, normal) * normal;
}

inline vec3 refract(
  const vec3& v, const vec3& normal, double eta_over_etap
) {
  // Theta is the angle of incidence.
  auto cos_theta = dot(-v, normal);
  // I don't understand why these give you the components of the refracted ray.
  vec3 refracted_perpendicular
    = eta_over_etap * (v + cos_theta * normal);
  vec3 refracted_parallel 
    = -sqrt(fabs(1.0 - refracted_perpendicular.length_squared())) * normal;
  return refracted_perpendicular + refracted_parallel;
}

inline vec3 unit_vector(vec3 v) {
  return v / v.length();
}

#endif