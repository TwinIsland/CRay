#pragma once

typedef struct Vec3 {
    double data[3];
} Vec3;

#define Vec3(x, y, z)                                                          \
    (Vec3) { x, y, z }

#define X(vec) vec.data[0]
#define Y(vec) vec.data[1]
#define Z(vec) vec.data[2]

Vec3 vec3_add(Vec3 v1, Vec3 v2);
Vec3 vec3_add3(Vec3 v1, Vec3 v2, Vec3 v3);
Vec3 vec3_add4(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4);
Vec3 vec3_min(Vec3 v1, Vec3 v2);
Vec3 vec3_min3(Vec3 v1, Vec3 v2, Vec3 v3);
Vec3 vec3_min4(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4);
double vec3_dot(Vec3 v1, Vec3 v2);
Vec3 vec3_cross(Vec3 v1, Vec3 v2);
Vec3 vec3_multiply(Vec3 v, double val);
Vec3 vec3_div(Vec3 v, double val);
double vec3_length(Vec3 v);
Vec3 vec3_normalize(Vec3 v);
Vec3 vec3_reflect(const Vec3 v, const Vec3 n);
double vec3_length_squared(Vec3 v);
Vec3 vec3_refract(const Vec3 uv, const Vec3 n, double etai_over_etat);