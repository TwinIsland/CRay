#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

extern char *errmsg;

Image image_init(int width, int height) {
  Image img = {.height = height, .width = width};
  img.datas = calloc(width * height * 3, sizeof(uint8_t));
  if (img.datas == NULL) {
    errmsg = "malloc failed";
  }
  return img;
}

inline void image_draw(Image *image, int x, int y, Color color) {
  if (x < 0 || x >= image->width || y < 0 || y >= image->height) {
    errmsg = "image out of bound";
    return;
  }

  image->datas[y * image->width + x] = color;
}

int image_save(Image *image, const char *filename) {
  return stbi_write_png(filename, image->width, image->height, 3, image->datas,
                        image->width * 3);
}

void image_free(Image *image) { free(image->datas); }
