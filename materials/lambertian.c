#include "../material.h"
#include "../utils.h"
#include <stdlib.h>

struct _lambertian_data {
    Albedo albedo;
};

bool scatter(const Ray *r_in, const Vec3 p, const Vec3 N, bool front_face,
             Albedo *attenuation, Ray *scattered, void *_data) {
    struct _lambertian_data *data = (struct _lambertian_data *)_data;

    Vec3 bounce_direction = vec3_add(N, gen_random_direction());
    *scattered = (Ray){.origin = p, .direction = bounce_direction};
    *attenuation = data->albedo;

    return true;
};

void lambertian_free(Material mat) { free(mat._data); }

Material mat_create_lambertian(const Albedo albedo) {
    struct _lambertian_data *data = malloc(sizeof(struct _lambertian_data));
    data->albedo = albedo;
    Material mat = {
        .name = "lambertian",
        .scatter = scatter,
        ._data = data,
        .free = lambertian_free,
    };
    return mat;
};