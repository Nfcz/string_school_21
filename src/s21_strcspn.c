#include "s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t len = 0;
  int flag = 1;
  for (int i = 0; str1[i] && flag; i++) {
    for (int j = 0; str2[j]; j++) {
      if (str1[i] == str2[j]) flag = 0;
    }
    if (flag == 1) len++;
  }
  return len;
}