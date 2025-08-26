#include <stdio.h>

#include "utils.h"

void print_progress(int current, int total) {
  const int bar_width = 50;
  float progress = (float)current / total;
  int filled_width = (int)(progress * bar_width);

  printf("\r[");
  for (int i = 0; i < filled_width; ++i) {
    printf("#");
  }
  for (int i = filled_width; i < bar_width; ++i) {
    printf(" ");
  }
  printf("] %.2f%%%c", progress * 100, current == total ? '\n' : ' ');
  fflush(stdout);
}
