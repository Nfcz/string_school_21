#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *ptr = s21_NULL;
  if (str == s21_NULL) str = ptr;
  while ((str != s21_NULL) && (*str != '\0') &&
         (s21_strchr(delim, *str) != s21_NULL)) {
    str++;
  }
  if (str && *str == '\0') {
    str = s21_NULL;
    ptr = s21_NULL;
  }
  if (str && *str != '\0') {
    ptr = str;
    while (*ptr && !(s21_strchr(delim, *ptr))) {
      ptr++;
    }
  }
  if (ptr && *ptr) {
    *ptr = '\0';
    ptr++;
  }
  return str;
}