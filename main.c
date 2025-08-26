#include <stdint.h>
#include <stdio.h>

#include "camera.h"
#include "image.h"
#include "material.h"
#include "object.h"
#include "world.h"

char *errmsg;
double aspect_ratio = 16.0 / 9.0;
int image_width = 1200;

static double random_double(void) { return (double)rand() / RAND_MAX; }

static double random_double_range(double min, double max) {
    return min + (max - min) * random_double();
}

static Albedo random_albedo(void) {
    return (Albedo){{random_double(), random_double(), random_double()}};
}

static Albedo random_albedo_range(double min, double max) {
    return (Albedo){{random_double_range(min, max),
                     random_double_range(min, max),
                     random_double_range(min, max)}};
}

int main() {
    // Camera setup
    Camera camera =
        camera_create(image_width,       
                      aspect_ratio, 
                      90, 
                      (Point3){5, 2, 3}, 
                      (Point3){0, 0, 0},  
                      (Vec3){0, 1, 0}, 
                      0.6,               
                      10.0,             
                      100     
        );

    // Output setup
    Image image = image_init(camera.image_width, camera.image_height);

    // World init
    World world = world_create();

    Material ground_material = mat_create_lambertian((Albedo){{0.5, 0.5, 0.5}});
    Object ground =
        obj_create_sphere((Vec3){0, -1000, 0}, 1000, ground_material);
    world_add(&world, ground);

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose_mat = random_double();
            Vec3 center = {a + 0.9 * random_double(), 0.2,
                           b + 0.9 * random_double()};

            Vec3 ref_point = {4, 0.2, 0};
            if (vec3_length(vec3_min(center, ref_point)) > 0.9) {
                Material sphere_material;

                if (choose_mat < 0.8) {
                    Albedo albedo = {random_double() * random_double(),
                                     random_double() * random_double(),
                                     random_double() * random_double()};
                    sphere_material = mat_create_lambertian(albedo);
                } else if (choose_mat < 0.95) {
                    Albedo albedo = random_albedo_range(0.5, 1.0);
                    double fuzz = random_double_range(0, 0.5);
                    sphere_material = mat_create_metal(albedo, fuzz);
                } else {
                    sphere_material = mat_create_dielectric(1.5);
                }

                Object small_sphere =
                    obj_create_sphere(center, 0.2, sphere_material);
                world_add(&world, small_sphere);
            }
        }
    }

    // Three large spheres
    Material material1 = mat_create_dielectric(1.5);
    Material material2 =
        mat_create_lambertian((Albedo){{0.4, 0.2, 0.1}}); 
    Material material3 =
        mat_create_metal((Albedo){{0.7, 0.6, 0.5}}, 0.0);

    Object sphere1 =
        obj_create_sphere((Vec3){0, 1, 0}, 1.0, material1); 
    Object sphere2 =
        obj_create_sphere((Vec3){-4, 1, 0}, 1.0, material2);
    Object sphere3 =
        obj_create_sphere((Vec3){4, 1, 0}, 1.0, material3);

    world_add(&world, sphere1);
    world_add(&world, sphere2);
    world_add(&world, sphere3);

    // Render
    printf("Image: %dx%d, Samples: %d, FOV: %.1f°\n", camera.image_width,
           camera.image_height, camera.samples_per_pixel, camera.vfov);

    camera_render(camera, &image, &world);

    // Save output
    int result = image_save(&image, "image.png");

    // Cleanup
    camera_free(&camera);
    world_free(&world);
    image_free(&image);

    if (result) {
        printf("Image saved successfully to tutorial_scene.png\n");
        return 0;
    } else {
        printf("Failed to save image\n");
        return 1;
    }
}