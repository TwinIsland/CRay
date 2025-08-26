#pragma once
#include <stdbool.h>

#include "point.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"
#include "hit_record.h"
#include "material.h"

typedef struct Object_t {
    char name[64];
    Material mat;
    bool (*hit)(const Ray *ray, Interval ray_t,
                hit_record *rec, void *_data);
    void (*free)(struct Object_t obj);
    void *data;
} Object;

///// ASSETS
// Sphere
Object obj_create_sphere(const Vec3 center, double radius, Material mat);
