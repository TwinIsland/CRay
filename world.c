#include "world.h"
#include <string.h>

#include "interval.h"

#define INITIAL_CAPACITY 4

World world_create() {
    World list = {0};
    list.objects = malloc(INITIAL_CAPACITY * sizeof(Object));
    if (list.objects) {
        list.capacity = INITIAL_CAPACITY;
        list.count = 0;
    }
    return list;
}


void world_free(World *world) {
    if (!world)
        return;

    for (size_t i = 0; i < world->count; i++) {
        if (world->objects[i].free) {
            world->objects[i].free(world->objects[i]);
        }
    }

    free(world->objects);

    world->objects = NULL;
    world->count = 0;
    world->capacity = 0;
}

void world_clear(World *list) {
    if (!list)
        return;

    for (size_t i = 0; i < list->count; i++) {
        if (list->objects[i].free) {
            list->objects[i].free(list->objects[i]);
        }
    }

    list->count = 0;
}

bool world_reserve(World *list, size_t new_capacity) {
    if (!list || new_capacity <= list->capacity) {
        return true;
    }

    Object *new_objects = realloc(list->objects, new_capacity * sizeof(Object));
    if (!new_objects) {
        return false;
    }

    list->objects = new_objects;
    list->capacity = new_capacity;
    return true;
}

bool world_add(World *list, Object obj) {
    if (!list)
        return false;

    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (!world_reserve(list, new_capacity)) {
            return false;
        }
    }

    list->objects[list->count] = obj;
    list->count++;
    return true;
}

bool world_hit(const World *list, const Ray *ray, Interval ray_t, hit_record *rec) {
    if (!list || !list->objects || list->count == 0) {
        return false;
    }

    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = ray_t.max;

    for (size_t i = 0; i < list->count; i++) {
        const Object *obj = &list->objects[i];

        if (obj->hit &&
            obj->hit(ray, (Interval) {ray_t.min, closest_so_far}, &temp_rec, obj->data)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            *rec = temp_rec;

        }
    }

    return hit_anything;
}