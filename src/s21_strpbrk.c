#include "s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  int flag = 0;
  char *result;
  s21_size_t n1 = s21_strlen(str1);
  s21_size_t n2 = s21_strlen(str2);
  for (int i = 0; i < (int)n1; i++) {
    for (int j = 0; j < (int)n2; j++) {
      if (str1[i] == str2[j]) {
        flag = 1;
        result = (char *)str1 + i;
        break;
      }
    }
    if (flag == 1) break;
  }
  if (flag == 0) result = s21_NULL;
  return result;
}
