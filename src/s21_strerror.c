#include <stdio.h>
#include <stdlib.h>

#include "s21_string.h"
char *s21_strerror(int errnum) {
  static char res[1024] = {'\0'};

  if (errnum >= count_error || errnum < 0) {
#if defined(__APPLE__) || (__MACH__)
    s21_sprintf(res, "Unknown error: %d", errnum);
#elif defined(__linux__)
    s21_sprintf(res, "Unknown error %d", errnum);
#endif
  } else
    s21_sprintf(res, "%s", s21_error_list[errnum]);

  return res;
}