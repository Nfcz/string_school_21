#include <stdlib.h>

#include "s21_string.h"
void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  if (src == s21_NULL || str == s21_NULL) return s21_NULL;

  s21_size_t n1 = s21_strlen(src);
  s21_size_t n2 = s21_strlen(str);

  if (start_index > n1) return s21_NULL;

  unsigned char *result = malloc(sizeof(unsigned char) * (n1 + n2 + 1));

  if (result == s21_NULL) return s21_NULL;

  s21_size_t n = 0;
  for (s21_size_t i = 0; i < n1 + 1; i++) {
    if (i == start_index) {
      for (s21_size_t j = 0; j < n2; j++) {
        result[n] = str[j];
        n += 1;
      }
    }
    result[n] = src[i];
    n += 1;
  }

  return result;
}
