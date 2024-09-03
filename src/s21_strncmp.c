#include "s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int sum = 0;
  unsigned char *string1 = (unsigned char *)str1;
  unsigned char *string2 = (unsigned char *)str2;
  for (s21_size_t i = 0; i < n; i++) {
    if ((string1[i] != string2[i]) || string1[i] == '\0' ||
        string2[i] == '\0') {
      sum = string1[i] - string2[i];
      break;
    }
  }
  return sum;
}
