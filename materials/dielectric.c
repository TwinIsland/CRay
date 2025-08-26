#include "../material.h"
#include "../utils.h"
#include <math.h>
#include <stdlib.h>

struct _dielectric_data {
    double refraction_index;
};

// Schlick's approximation for reflectance
static double reflectance(double cosine, double ref_idx) {
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool dielectric_scatter(const Ray *r_in, const Vec3 p, const Vec3 N,
                        bool front_face, Albedo *attenuation, Ray *scattered,
                        void *_data) {
    struct _dielectric_data *data = (struct _dielectric_data *)_data;

    *attenuation = (Albedo){{1.0, 1.0, 1.0}};

    double refraction_ratio =
        front_face ? (1.0 / data->refraction_index) : data->refraction_index;

    Vec3 unit_direction = vec3_normalize(r_in->direction);
    double cos_theta =
        fmin(vec3_dot(vec3_multiply(unit_direction, -1.0), N), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    Vec3 direction;

    if (cannot_refract ||
        reflectance(cos_theta, refraction_ratio) > random_double()) {
        direction = vec3_reflect(unit_direction, N);
    } else {
        direction = vec3_refract(unit_direction, N, refraction_ratio);
    }

    *scattered = (Ray){.origin = p, .direction = direction};
    return true;
}

void dielectric_free(Material mat) { free(mat._data); }

Material mat_create_dielectric(double refraction_index) {
    struct _dielectric_data *data = malloc(sizeof(struct _dielectric_data));
    data->refraction_index = refraction_index;

    Material mat = {
        .name = "dielectric",
        .scatter = dielectric_scatter,
        ._data = data,
        .free = dielectric_free,
    };
    return mat;
}
