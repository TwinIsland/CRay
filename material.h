#pragma once
#include <stdbool.h>

#include "color.h"
#include "ray.h"

typedef struct {
    double data[3];
} Albedo;

typedef struct Material_t {
    char name[64];
    bool (*scatter)(const Ray *r_in, const Vec3 p, const Vec3 N, bool front_face, Albedo *attenuation,
             Ray *scattered, void *_data);
    void (*free)(struct Material_t mat);
    void *_data;
} Material;


#define Albedo(r, g, b)                                                         \
    (Albedo) { r, g, b }

#define ALB_R(r) r.data[0]
#define ALB_G(r) r.data[1]
#define ALB_B(r) r.data[2]

///// GLOBAL
#define MAT_FREE(mat) mat.free(mat)

///// ASSETS
Material mat_create_lambertian(const Albedo albedo);
Material mat_create_metal(const Albedo albedo, double fuzz);
Material mat_create_dielectric(double refraction_index);