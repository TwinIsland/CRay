#pragma once

#include "vec3.h"
#include "point.h"

typedef struct {
  Vec3 origin;
  Vec3 direction;
} Ray;


static inline Point3 ray_at(Ray ray, double t) {
    return (Point3) (vec3_add(ray.origin, vec3_multiply(ray.direction, t)));
}
