#pragma once

#include "vec3.h"
#include <math.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <stdlib.h>
#include <stdbool.h>

// constants
#define INFINITY_RT DBL_MAX
#define PI 3.1415926535897932385

static inline double degrees_to_radians(double degrees) {
    return degrees * PI / 180.0;
}

static inline double radians_to_degrees(double radians) {
    return radians * 180.0 / PI;
}

static inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

static inline double random_double(void) { return (double)rand() / RAND_MAX; }

static inline double random_double_range(double min, double max) {
    return min + (max - min) * random_double();
}

// generate random offset for pixel (range: -0.5 to +0.5)
static inline Vec3 sample_square(void) {
    return (Vec3){random_double() - 0.5, random_double() - 0.5, 0.0};
}

// generate random direction in the same direction of N (unit vector)
static inline Vec3 gen_random_direction() {
    while (true) {
        Vec3 p =
            Vec3(random_double_range(-1.0, 1.0), random_double_range(-1.0, 1.0),
                 random_double_range(-1.0, 1.0));

        double lensq = vec3_dot(p, p);

        if (1e-160 < lensq && lensq <= 1.0) {
            return vec3_div(p, sqrt(lensq));
        }
    }
}

// progress bar
void print_progress(int current, int total);
