#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  unsigned char *string = (unsigned char *)str;
  int flag = 1;
  char *result;

  for (int i = 0; i < (int)n; i++) {
    if (string[i] == (char)c) {
      flag = 0;
      result = (char *)str + i;
      break;
    }
  }

  if (flag == 1) result = s21_NULL;

  return result;
}
