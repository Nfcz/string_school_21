#include "s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  s21_size_t n_dest = s21_strlen(dest);
  s21_size_t j = 0;

  for (s21_size_t i = n_dest; i < n_dest + n; i++) {
    if (src[j] == '\0') break;
    dest[i] = src[j];
    j++;
  }
  dest[n_dest + j] = '\0';

  return dest;
}