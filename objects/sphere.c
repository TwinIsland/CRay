#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../object.h"
#include "../interval.h"

struct _sphere_data {
    Vec3 center;
    double radius;
    Material mat;
};

static bool sphere_hit(const Ray *ray, Interval ray_t,
                       hit_record *rec, void *_data) {
    struct _sphere_data *data = (struct _sphere_data *)_data;

    Vec3 oc = vec3_min(data->center, ray->origin);
    double a = vec3_dot(ray->direction, ray->direction);
    double h = vec3_dot(ray->direction, oc);
    double c = vec3_dot(oc, oc) - data->radius * data->radius;
    double discriminant = h * h - a * c;

    if (discriminant < 0)
        return false;

    double sqrtd = sqrt(discriminant);
    double root = (h - sqrtd) / a;
    if (!interval_surrounds(&ray_t, root)) {
        root = (h + sqrtd) / a;
        if (!interval_surrounds(&ray_t, root)) {
            return false;
        }
    }

    rec->mat = data->mat;
    rec->t = root;
    rec->p = ray_at(*ray, rec->t);
    // check frontface
    Vec3 outward_normal = vec3_div(vec3_min(rec->p, data->center), data->radius);
    rec->front_face = vec3_dot(ray->direction, outward_normal) < 0;
    rec->N = rec->front_face ? outward_normal : vec3_multiply(outward_normal, -1.0);
    return true;
}

static void sphere_free(Object sphere) { free(sphere.data); }

Object obj_create_sphere(const Vec3 center, double radius, Material mat) {
    struct _sphere_data *data = malloc(sizeof(struct _sphere_data));
    data->center = center;
    data->radius = radius;
    data->mat = mat;

    Object obj =  (Object){
        .mat = mat,
        .hit = sphere_hit,
        .data = (void *)data,
        .free = sphere_free,
    };
    
    snprintf(obj.name, sizeof(obj.name), "sphere (r=%f\tcenter=(%f, %f, %f)", radius, X(center), Y(center), Z(center));
    
    return obj;
}
