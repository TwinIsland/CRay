#include "vec3.h"
#include <math.h>

inline Vec3 vec3_add(Vec3 v1, Vec3 v2) {
    Vec3 result;
    result.data[0] = v1.data[0] + v2.data[0];
    result.data[1] = v1.data[1] + v2.data[1];
    result.data[2] = v1.data[2] + v2.data[2];
    return result;
}

inline Vec3 vec3_add3(Vec3 v1, Vec3 v2, Vec3 v3) {
    return vec3_add(vec3_add(v1, v2), v3);
}

inline Vec3 vec3_add4(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4) {
    return vec3_add(vec3_add3(v1, v2, v3), v4);
}

inline Vec3 vec3_min(Vec3 v1, Vec3 v2) {
    Vec3 result;
    result.data[0] = v1.data[0] - v2.data[0];
    result.data[1] = v1.data[1] - v2.data[1];
    result.data[2] = v1.data[2] - v2.data[2];
    return result;
}

inline Vec3 vec3_min3(Vec3 v1, Vec3 v2, Vec3 v3) {
    return vec3_min(vec3_min(v1, v2), v3);
}

inline Vec3 vec3_min4(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4) {
    return vec3_min(vec3_min3(v1, v2, v3), v4);
}

inline double vec3_dot(Vec3 v1, Vec3 v2) {
    return v1.data[0] * v2.data[0] + v1.data[1] * v2.data[1] +
           v1.data[2] * v2.data[2];
}

inline Vec3 vec3_cross(Vec3 v1, Vec3 v2) {
    Vec3 result;
    result.data[0] = v1.data[1] * v2.data[2] - v1.data[2] * v2.data[1];
    result.data[1] = v1.data[2] * v2.data[0] - v1.data[0] * v2.data[2];
    result.data[2] = v1.data[0] * v2.data[1] - v1.data[1] * v2.data[0];
    return result;
}

inline Vec3 vec3_multiply(Vec3 v, double val) {
    Vec3 result;
    result.data[0] = v.data[0] * val;
    result.data[1] = v.data[1] * val;
    result.data[2] = v.data[2] * val;
    return result;
}

inline Vec3 vec3_div(Vec3 v, double val) {
    Vec3 result;
    result.data[0] = v.data[0] / val;
    result.data[1] = v.data[1] / val;
    result.data[2] = v.data[2] / val;
    return result;
}

inline double vec3_length(Vec3 v) {
    return sqrt(v.data[0] * v.data[0] + v.data[1] * v.data[1] +
                v.data[2] * v.data[2]);
}

inline Vec3 vec3_normalize(Vec3 v) {
    double length = vec3_length(v);
    if (length == 0.0) {
        return (Vec3){{0.0, 0.0, 0.0}};
    }
    return vec3_div(v, length);
}

inline Vec3 vec3_reflect(const Vec3 v, const Vec3 n) {
    return vec3_min(v, vec3_multiply(n, 2 * vec3_dot(v, n)));
}

inline double vec3_length_squared(Vec3 v) {
    return v.data[0] * v.data[0] + v.data[1] * v.data[1] + v.data[2] * v.data[2];
}

inline Vec3 vec3_refract(const Vec3 uv, const Vec3 n, double etai_over_etat) {
    double cos_theta = fmin(vec3_dot(vec3_multiply(uv, -1.0), n), 1.0);
    Vec3 r_out_perp = vec3_multiply(vec3_add(uv, vec3_multiply(n, cos_theta)), etai_over_etat);
    Vec3 r_out_parallel = vec3_multiply(n, -sqrt(fabs(1.0 - vec3_length_squared(r_out_perp))));
    return vec3_add(r_out_perp, r_out_parallel);
}
