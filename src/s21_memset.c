#include "s21_string.h"

void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *string = (unsigned char *)str;

  for (int i = 0; i < (int)n; i++) {
    string[i] = c;
  }

  return str;
}