#include "s21_string.h"

char *s21_strrchr(const char *str, int c) {
  s21_size_t n = s21_strlen(str);
  int flag = 0;
  char *result;
  for (int i = 0; i <= (int)n; i++) {
    if (str[i] == (char)c) {
      flag = 1;
      result = (char *)str + i;
    }
  }

  if (flag == 0) result = s21_NULL;

  return result;
}