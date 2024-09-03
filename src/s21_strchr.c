#include "s21_string.h"

char *s21_strchr(const char *str, int c) {
  s21_size_t n = s21_strlen(str);
  int token = -1;
  char *result;
  for (int i = 0; i <= (int)n; i++) {
    if (str[i] == (char)c) {
      token = (int)i;
      result = (char *)str + i;
      break;
    }
  }

  if (token == -1) result = s21_NULL;

  return result;
}
