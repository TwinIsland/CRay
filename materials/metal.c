#include "../material.h"
#include "../utils.h"
#include <stdlib.h>

struct _metal_data {
    Albedo albedo;
    double fuzz;
};

bool metal_scatter(const Ray *r_in, const Vec3 p, const Vec3 N, bool front_face,
                   Albedo *attenuation, Ray *scattered, void *_data) {
    struct _metal_data *data = (struct _metal_data *)_data;

    Vec3 reflected = vec3_reflect(vec3_normalize(r_in->direction), N);

    if (data->fuzz > 0) {
        reflected = vec3_add(reflected,
                             vec3_multiply(gen_random_direction(), data->fuzz));
    }

    *scattered = (Ray){.origin = p, .direction = reflected};
    *attenuation = data->albedo;

    return vec3_dot(scattered->direction, N) > 0;
}

void metal_free(Material mat) { free(mat._data); }

Material mat_create_metal(const Albedo albedo, double fuzz) {
    struct _metal_data *data = malloc(sizeof(struct _metal_data));
    data->albedo = albedo;
    data->fuzz = fuzz < 1.0 ? fuzz : 1.0;

    Material mat = {
        .name = "metal",
        .scatter = metal_scatter,
        ._data = data,
        .free = metal_free,
    };
    return mat;
}
