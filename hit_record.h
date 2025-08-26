#pragma once
#include <stdbool.h>

#include "point.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"
#include "material.h"

typedef struct {
    Point3 p;
    Vec3 N;
    double t;
    bool front_face;
    Material mat;
} hit_record;