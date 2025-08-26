#pragma once

#include "color.h"
#include <stdint.h>

typedef struct {
  int width;
  int height;
  Color *datas;
} Image;

Image image_init(int width, int height);
void image_draw(Image *image, int x, int y, Color color);
int image_save(Image *image, const char *filename);
void image_free(Image *image);
