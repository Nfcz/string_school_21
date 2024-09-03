#include <stdlib.h>
#include <string.h>

#include "s21_string.h"

void *s21_to_lower(const char *str) {
  if (str == s21_NULL) return s21_NULL;
  s21_size_t n = s21_strlen(str) + 1;

  unsigned char *buffer = malloc(sizeof(unsigned char) * n);
  if (buffer == s21_NULL) return s21_NULL;
  buffer = (unsigned char *)s21_memset((void *)buffer, '\0', n);
  unsigned char *str1 = (unsigned char *)str;

  for (s21_size_t i = 0; i < n; i++) {
    if (str[i] >= 'A' && str[i] <= 'Z')
      buffer[i] = str1[i] + 32;
    else
      buffer[i] = str1[i];
  }

  return (void *)buffer;
}