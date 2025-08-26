#include "camera.h"
#include "color.h"
#include "material.h"
#include "math.h"
#include "point.h"
#include "utils.h"
#include "vec3.h"

static Vec3 random_in_unit_disk(void) {
    while (true) {
        Vec3 p = (Vec3){random_double_range(-1.0, 1.0),
                        random_double_range(-1.0, 1.0), 0.0};
        if (vec3_length_squared(p) < 1.0) {
            return p;
        }
    }
}

static Point3 defocus_disk_sample(const Camera *camera) {
    Vec3 p = random_in_unit_disk();
    return vec3_add3(camera->center,
                     vec3_multiply(camera->defocus_disk_u, X(p)),
                     vec3_multiply(camera->defocus_disk_v, Y(p)));
}

static Ray get_ray(const Camera *camera, int i, int j) {
    Vec3 offset = sample_square();

    Point3 pixel_sample =
        vec3_add3(camera->pixel00_loc,
                  vec3_multiply(camera->pixel_delta_u, i + X(offset)),
                  vec3_multiply(camera->pixel_delta_v, j + Y(offset)));

    Point3 ray_origin = (camera->defocus_angle <= 0)
                            ? camera->center
                            : defocus_disk_sample(camera);

    Vec3 ray_direction = vec3_min(pixel_sample, ray_origin);

    return (Ray){.origin = ray_origin, .direction = ray_direction};
}

static Albedo ray_color_recursive(const Ray *ray, const World *world,
                                  int depth) {
    if (depth <= 0) {
        return (Albedo){0, 0, 0}; // set to black if bounce too many times
    }

    hit_record rec;

    if (world_hit(world, ray, (Interval){0.001, INFINITY}, &rec)) {
        Ray scattered;
        Albedo attenuation;

        if (rec.mat.scatter(ray, rec.p, rec.N, rec.front_face, &attenuation,
                            &scattered, rec.mat._data)) {
            Albedo bounced_albedo =
                ray_color_recursive(&scattered, world, depth - 1);
            return Albedo(ALB_R(attenuation) * R(bounced_albedo),
                          ALB_G(attenuation) * G(bounced_albedo),
                          ALB_B(attenuation) * B(bounced_albedo));
        }
        return (Albedo){0, 0, 0};
    } else {
        // no hit, render background directly
        Vec3 unit_ray_direction = vec3_normalize(ray->direction);
        double scaler = 0.5 * (Y(unit_ray_direction) + 1.0);

        Vec3 white = Vec3(1.0, 1.0, 1.0);
        Vec3 blue = Vec3(0.5, 0.7, 1.0);
        Vec3 color_vec = vec3_add(vec3_multiply(white, (1.0 - scaler)),
                                  vec3_multiply(blue, scaler));

        return (Albedo){
            X(color_vec),
            Y(color_vec),
            Z(color_vec),
        };
    }
}

static double linear_to_gamma(double linear_component) {
    if (linear_component > 0) {
        return sqrt(linear_component);
    }
    return 0.0;
}

static Color ray_color(const Ray *ray, const World *world) {
    Albedo final_albedo = ray_color_recursive(ray, world, MAX_RAY_BOUNCES);

    return Color(clamp(linear_to_gamma(ALB_R(final_albedo)) * 255, 0, 255),
                 clamp(linear_to_gamma(ALB_G(final_albedo)) * 255, 0, 255),
                 clamp(linear_to_gamma(ALB_B(final_albedo)) * 255, 0, 255));
}

Camera camera_create(double image_width, double aspect_ratio, double vfov,
                     Point3 lookfrom, Point3 lookat, Vec3 vup,
                     double defocus_angle, double focus_dist,
                     int samples_per_pixel) {
    int image_height = (int)(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    double focal_length = vec3_length(vec3_min(lookfrom, lookat));
    Point3 camera_center = lookfrom;

    // Calculate viewport from FOV (like tutorial)
    double theta = degrees_to_radians(vfov);
    double h = tan(theta / 2.0);
    double viewport_height = 2.0 * h * focal_length;
    double viewport_width =
        viewport_height * ((double)image_width / image_height);

    Vec3 w = vec3_normalize(vec3_min(lookfrom, lookat)); // Camera back
    Vec3 u = vec3_normalize(vec3_cross(vup, w));         // Camera right
    Vec3 v = vec3_cross(w, u);                           // Camera up

    // displacement vector for viewport and pixels
    Vec3 viewport_u = vec3_multiply(u, viewport_width);
    Vec3 viewport_v = vec3_multiply(v, -viewport_height);
    Vec3 pixel_delta_u = vec3_div(viewport_u, image_width);
    Vec3 pixel_delta_v = vec3_div(viewport_v, image_height);

    // upper left pixel
    Point3 viewport_upper_left =
        vec3_min4(camera_center, Vec3(0, 0, focal_length),
                  vec3_div(viewport_u, 2), vec3_div(viewport_v, 2));
    Point3 pixel00_loc =
        vec3_add(viewport_upper_left,
                 vec3_multiply(vec3_add(pixel_delta_u, pixel_delta_v), 2));

    // defocus disk basis vectors
    double defocus_radius =
        focus_dist * tan(degrees_to_radians(defocus_angle / 2.0));
    Vec3 defocus_disk_u = vec3_multiply(u, defocus_radius);
    Vec3 defocus_disk_v = vec3_multiply(v, defocus_radius);

    Camera camera = {
        .aspect_ratio = aspect_ratio,
        .image_width = image_width,
        .image_height = image_height,
        .samples_per_pixel = samples_per_pixel,
        .pixel_samples_scale = 1.0 / samples_per_pixel,
        .vfov = vfov,
        .lookfrom = lookfrom,
        .lookat = lookat,
        .vup = vup,
        .center = camera_center,
        .pixel00_loc = pixel00_loc,
        .pixel_delta_u = pixel_delta_u,
        .pixel_delta_v = pixel_delta_v,
        .u = u,
        .v = v,
        .w = w,
        .defocus_angle = defocus_angle,
        .defocus_disk_u = defocus_disk_u,
        .defocus_disk_v = defocus_disk_v,
    };

    return camera;
}

void camera_free(Camera *camera) { return; }

void camera_render(Camera camera, Image *dest, const World *world) {
    for (int j = 0; j < camera.image_height; ++j) {
        print_progress(j + 1, camera.image_height);
        for (int i = 0; i < camera.image_width; ++i) {
            Vec3 pixel_color = (Vec3){0, 0, 0};

            // super sampling
            for (int sample = 0; sample < camera.samples_per_pixel; sample++) {
                Ray sample_ray = get_ray(&camera, i, j);
                Color sample_color = ray_color(&sample_ray, world);

                pixel_color =
                    vec3_add(pixel_color, Vec3(R(sample_color), G(sample_color),
                                               B(sample_color)));
            }

            pixel_color =
                vec3_multiply(pixel_color, camera.pixel_samples_scale);

            Color final_color = {clamp(X(pixel_color), 0, 255),
                                 clamp(Y(pixel_color), 0, 255),
                                 clamp(Z(pixel_color), 0, 255)};

            image_draw(dest, i, j, final_color);
        }
    }
}
