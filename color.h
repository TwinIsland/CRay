#pragma once

#include "color.h"
#include "vec3.h"
#include <stdint.h>

typedef struct {
    uint8_t data[3];
} Color;

#define Color(r, g, b)                                                         \
    (Color) { r, g, b }

#define R(color) color.data[0]
#define G(color) color.data[1]
#define B(color) color.data[2]
