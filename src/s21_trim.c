#include <stdlib.h>

#include "s21_string.h"

void *s21_trim(const char *src, const char *trim_chars) {
  if (src == s21_NULL) return s21_NULL;

  if (trim_chars == s21_NULL || s21_strlen(trim_chars) == 0)
    trim_chars = " \n\t\v\f\r";

  char *result = s21_NULL;
  s21_size_t n = s21_strlen(src);
  int f1 = 0;
  int f2 = n - 1;

  while (f1 < f2 && src[f1] && s21_strchr(trim_chars, (int)src[f1])) {
    f1++;
  }

  while (f1 < f2 && src[f2] && s21_strchr(trim_chars, (int)src[f2])) {
    f2--;
  }

  if (f1 < f2) {
    result = (char *)malloc((f2 - f1 + 2) * sizeof(char));

    if (!(result)) return s21_NULL;

    int i = 0;
    for (int x = f1; x <= f2; x++) {
      result[i++] = src[x];
    }
    result[i] = '\0';
  }

  if (f1 == f2 || s21_strlen(src) == 0) {
    result = malloc(sizeof(char));
    result[0] = '\0';
  }

  return result;
}