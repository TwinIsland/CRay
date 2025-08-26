#pragma once

#include "image.h"
#include "point.h"
#include "vec3.h"
#include "world.h"

#define MAX_RAY_BOUNCES 50

typedef struct {
    const double aspect_ratio;
    const int image_width;
    const int image_height;
    const Point3 center;
    const Point3 pixel00_loc;
    const Vec3 pixel_delta_u;
    const Vec3 pixel_delta_v;
    int samples_per_pixel;      // for supersampling
    double pixel_samples_scale; // 1 / samples_per_pixel
    double vfov;

    Point3 lookfrom; // Camera position
    Point3 lookat;   // Point camera looks at
    Vec3 vup;        // Camera "up" direction

    Vec3 u, v, w;

    double defocus_angle;
    double focus_dist;
    Vec3 defocus_disk_u; 
    Vec3 defocus_disk_v; 
} Camera;

Camera camera_create(double image_width, double aspect_ratio, double vfov,
                     Point3 lookfrom, Point3 lookat, Vec3 vup,
                     double defocus_angle, double focus_dist,
                     int samples_per_pixel);
void camera_free(Camera *camera);
void camera_render(Camera camera, Image *dest, const World *world);
