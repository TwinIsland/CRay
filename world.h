#pragma once

#include <stdlib.h>

#include "object.h"
#include "interval.h"

typedef struct {
    Object *objects;
    size_t count;
    size_t capacity;
} World;

World world_create();
void world_free(World *world);

void world_clear(World *list);
bool world_add(World *list, Object obj);
bool world_reserve(World *list, size_t new_capacity);

bool world_hit(const World *list, const Ray *ray, Interval ray_t, hit_record *rec);
