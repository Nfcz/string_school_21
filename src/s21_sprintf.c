#include "s21_sprintf.h"

#include <math.h>
#define MINUS 1
#define PLUS 2
#define SPASE 4
#define HASH 8
#define ZERO 16

int s21_sprintf(char *str, const char *format, ...) {
  int format_i = 0;
  char *str_cursor = str;
  va_list input;
  va_start(input, format);
  spec spec = parser(format);
  while (format[format_i]) {
    for (; (spec.start != s21_NULL && format_i < -(format - spec.start)) ||
           (spec.start == s21_NULL && format[format_i]);
         format_i++) {
      *str_cursor = format[format_i];
      str_cursor++;
    }
    specifier_to_str(spec, str, &str_cursor, &input);
    if (spec.end != s21_NULL) {
      format_i = spec.end - format + 1;
    }
#ifdef __linux__
    if (spec.error) str[0] = '\0';
#endif
    spec = parser(format + format_i);
  }
  *str_cursor = '\0';
  va_end(input);
  return (int)(str_cursor - str);
}

spec parser(const char *format) {
  spec spec = {0};
  // spec.start = spec.end = s21_NULL;
  // spec.l = spec.sp = '\0';
  spec.w = spec.pre = -1;
  // spec.fl = 0;
  // spec.error = 0;
  char *start = s21_NULL;
  if ((spec.start = s21_strchr(format, '%')) != s21_NULL) {
    start = spec.start + 1;
    if (*start == '%') {
      spec.sp = '%';
      spec.end = start;
    } else if ((spec.end = s21_strpbrk(start, "ceEfgGosuxXpnid")) != s21_NULL) {
      spec.sp = *spec.end;
      int index = 0;
      index = s21_strspn(start, "-+ #0");
      str_to_flags(start, index, &spec.fl);
      start += index;

      width_parser(&spec, &start);
      precision_parser(&spec, &start);

      if (s21_strpbrk(start, "hlL") == start) {
        spec.l = *start;
        start++;
      }
    }
  }
#ifndef __linux__
  if (spec.sp != '\0' && start != spec.end) {
    spec.error = 1;
  }
#endif
  spec_preprocessing(&spec);
  return spec;
}

void width_parser(spec *spec, char **start) {
  int index = 0;
  if (**start == '*') {
    spec->w = -10001;
    (*start)++;
  } else {
    index = s21_strspn(*start, "0123456789");
    if (str_to_int_parser(*start, &spec->w, index) != 1) {
      spec->error = 1;
    }
    *start += index;
  }
}

void precision_parser(spec *spec, char **start) {
  int index = 0;
  if (**start == '.') {
    (*start)++;
    if (**start == '*') {
      spec->pre = -10001;
      (*start)++;
    } else {
      index = s21_strspn(*start, "-0123456789");
      if (str_to_int_parser(*start, &spec->pre, index) != 1) {
        spec->error = 1;
        spec->pre = -1;
      }
      if (index == 0) {
        spec->pre = 0;
      }
      if (spec->pre < 0) {
        spec->fl = spec->fl | MINUS;
        spec->w = spec->pre * -1;
        spec->pre = 0;
      }
      *start += index;
    }
  }
}

void spec_preprocessing(spec *spec) {
  if (spec->pre == -1) {
    switch (spec->sp) {
      case 'd':
      case 'i':
      case 'o':
      case 'u':
      case 'x':
      case 'X':
      case 'p':
        spec->pre = 1;
        break;
      case 'f':
      case 'e':
      case 'E':
      case 'g':
      case 'G':
        spec->pre = 6;
        break;
    }
  }

  // can't ' ' with %u (% u)
  // can't '+' with ' '
}

void specifier_to_str(spec spec, char *start, char **cursor, va_list *input) {
  star_processing(&spec, input);
  char *start_print_spec = *cursor;
  if (spec.sp == '%') {
    **cursor = '%';
    *cursor += 1;
  }
  if (spec.sp == 'c') {
    **cursor = va_arg(*input, int);
    *cursor += 1;
  }
  if (spec.sp == 's') s_to_str(spec, cursor, input);
  if (spec.sp == 'd' || spec.sp == 'i') {
    d_to_str(spec, cursor, input);
  }
  if (spec.sp == 'u') u_to_str(spec, cursor, input, 10, '\0');
  if (spec.sp == 'o') u_to_str(spec, cursor, input, 8, '\0');
  if (spec.sp == 'x') u_to_str(spec, cursor, input, 16, '\0');
  if (spec.sp == 'X') u_to_str(spec, cursor, input, 16, 'X');
  if (spec.sp == 'p') p_to_str(spec, cursor, input);
  if (spec.sp == 'n') {
    int *n = va_arg(*input, int *);
    *n = (int)(*cursor - start);
  }
  if (spec.sp == 'f') {
    double n = va_arg(*input, double);
    float_to_str(cursor, n, (spec.pre == -1) ? 6 : spec.pre, spec.fl);
  }
  // if (spec.sp == 'e') {
  //   double n = va_arg(*input, double);
  //   float_to_mantissa_str(cursor, n, 6, 'e');
  // }
  if (*cursor - start_print_spec < spec.w) {
    width_and_minus_to_str(spec, cursor, start_print_spec);
  }
}

void star_processing(spec *spec, va_list *input) {
  if (spec->w == -10001) {
    spec->w = va_arg(*input, int);
    if (spec->w < 0) {
      spec->w *= -1;
      spec->fl = spec->fl | MINUS;
    }
  }
  if (spec->pre == -10001) {
    spec->pre = va_arg(*input, int);
  }
}

void s_to_str(spec spec, char **cursor, va_list *input) {
  char *str = va_arg(*input, char *);
  int len = s21_strlen(str);
  if (spec.pre != -1 && len > spec.pre) {
    len = spec.pre;
  }
  for (int i = 0; i < len; i++) {
    **cursor = str[i];
    *cursor += 1;
  }
}

void d_to_str(spec spec, char **cursor, va_list *input) {
  long int n = {0};
  if (spec.l == 'l') {
    n = va_arg(*input, long int);
  } else if (spec.l == 'h') {
    n = (long int)((short int)va_arg(*input, int));
  } else {
    n = (long int)va_arg(*input, int);
  }
  int_to_str_10(cursor, n, spec.fl, spec.pre);
}

void u_to_str(spec spec, char **cursor, va_list *input, int counting_system,
              char flag) {
  long unsigned int n;
  if (spec.l == 'l') {
    n = va_arg(*input, long unsigned int);
  } else if (spec.l == 'h') {
    n = (long unsigned int)((short unsigned int)va_arg(*input,
                                                       long unsigned int));
  } else {
    n = (long unsigned int)va_arg(*input, unsigned int);
  }
  unsigned_int_to_str(cursor, (unsigned long long int)n, counting_system, flag,
                      spec.pre);
}

void p_to_str(spec spec, char **cursor, va_list *input) {
  long long int n = va_arg(*input, long long int);
#ifdef __linux__
  if (n == 0) {
    **cursor = '(';
    *cursor += 1;
    **cursor = 'n';
    *cursor += 1;
    **cursor = 'i';
    *cursor += 1;
    **cursor = 'l';
    *cursor += 1;
    **cursor = ')';
    *cursor += 1;
  } else {
    if (spec.fl & PLUS) {
      **cursor = '+';
      *cursor += 1;
    }
    **cursor = '0';
    *cursor += 1;
    **cursor = 'x';
    *cursor += 1;
    unsigned_int_to_str(cursor, (unsigned long long int)n, 16, '\0', spec.pre);
  }

#endif

#ifndef __linux__
  **cursor = '0';
  *cursor += 1;
  **cursor = 'x';
  *cursor += 1;
  unsigned_int_to_str(cursor, (unsigned long long int)n, 16, '\0', spec.pre);
#endif
}

void width_and_minus_to_str(spec spec, char **cursor, char *start_print_spec) {
  int len = (int)(*cursor - start_print_spec);
  int shift = spec.w - len;
  char my_char = ' ';
  if (!(spec.fl & MINUS) && (spec.fl & ZERO) &&
      (s21_strchr("iduoxX", spec.sp) != s21_NULL)) {
    my_char = '0';
    if (start_print_spec[0] == '-') {
      start_print_spec++;
      len--;
    }
  }
  for (int i = 1; i <= len && !(spec.fl & MINUS); i++) {
    start_print_spec[len + shift - i] = start_print_spec[len - i];
  }
  for (int i = 0; i < shift; i++) {
    start_print_spec[(spec.fl & MINUS) ? len + i : i] = my_char;
  }
  *cursor += shift;
}

int one_digit_str_to_int(char cursor) {
  int res = -1;
  if (cursor <= 57 && cursor >= 48) {
    res = cursor - 48;
  }
  return res;
}

int str_to_int_parser(char *str, int *res, int len) {
  int good = 1, sign = 1;
  if (len != 0) {
    *res = 0;
  }
  if (*str == '-') {
    len--;
    sign = -1;
    str++;
  }
  for (int i = 0; i < len; i++) {
    *res = *res * 10 + one_digit_str_to_int(str[i]);
    if (one_digit_str_to_int(str[i]) == -1) {
      good = 0;
    }
  }
  *res *= sign;
  return good;
}

void one_digit_int_to_str(char **cursor, int n, char flag) {
  if (n < 10 && n > -1) {
    **cursor = n + 48;
    *cursor += 1;
  } else if (n >= 10 && n < 16) {
    **cursor = n + ((flag == 'X') ? 55 : 87);
    *cursor += 1;
  } else {
    // printf("error!!(%d)\n", n);
  }
}

void int_to_str_10(char **cursor, long int n, int flag, int pre) {
  int first_digit = 0;
  if (n < 0) {
    n = n * -1;
    **cursor = '-';
    *cursor += 1;
  } else if (flag & PLUS) {
    **cursor = '+';
    *cursor += 1;
  } else if (flag & SPASE) {
    **cursor = ' ';
    *cursor += 1;
  }

  int max_p = 19;
  for (int i = 0; i + max_p < pre; i++) {
    **cursor = '0';
    *cursor += 1;
  }

  for (int i = max_p - 1; i >= 0; i--) {
    if (n >= pow(10, i)) {
      first_digit = 1;
      one_digit_int_to_str(cursor, (int)(n / pow(10, i)), '\0');
      n = n % (unsigned long int)pow(10, i);
    } else if (first_digit == 1 || (i < pre)) {
      **cursor = '0';
      *cursor += 1;
    }
  }
}

void unsigned_int_to_str(char **cursor, unsigned long long int n,
                         int counting_system, char flag, int pre) {
  int first_digit = 0;

  int max_p = max_pow(counting_system);
  for (int i = 0; i + max_p < pre; i++) {
    **cursor = '0';
    *cursor += 1;
  }

  for (int i = max_p - 1; i >= 0; i--) {
    unsigned long long int pow_our = 1;
    for (int k = 0; k < i; k++) {
      pow_our *= counting_system;
    }
    if (n >= pow_our) {
      first_digit = 1;
      int digit = (int)(n / pow_our);
      one_digit_int_to_str(cursor, digit, flag);
      n = n % pow_our;
    } else {
      if (first_digit == 1 || i < pre) {
        **cursor = '0';
        *cursor += 1;
      }
    }
  }
}

void float_to_str(char **cursor, double n, int len, int flag) {
  long long int *p = (long long int *)&n;
  if (*p < 0) {
    **cursor = '-';
    *cursor += 1;
    n = n * -1;
  } else if (flag & PLUS) {
    **cursor = '+';
    *cursor += 1;
  } else if (flag & SPASE) {
    **cursor = ' ';
    *cursor += 1;
  }

  double integer = {0};
  double fraction = {0};
  fraction = modf(n, &integer);
  integer += round_fraction(&fraction, len);

  integer_print(cursor, integer);

  if (len > 0) {
    **cursor = '.';
    *cursor += 1;
  }

  fraction_print(cursor, fraction, len);
}

// void float_to_mantissa_str(char **cursor, double n, int len, char flag) {
//   long long int *p = (long long int *)&n;
//   if (*p < 0) {
//     **cursor = '-';
//     *cursor += 1;
//     n = n * -1;
//   }
//   int power = digit_counter(n);
//   double fraction;
//   int integer = break_double_apart_e_form(n, &fraction, power, len);
//   one_digit_int_to_str(cursor, integer, '\0');
//   if (len > 0) {
//     **cursor = '.';
//     *cursor += 1;
//   }
//   fraction_print(cursor, fraction, len);
//   **cursor = flag;
//   *cursor += 1;
//   int_to_str_10(cursor, power - 1, PLUS, 2);
// }

int digit_counter(double n) {
  int i = {0};
  for (i = 309; i > -309 && n < pow(10, i); i--)
    ;
  long double digit_double =
      (fmodl(n, powl(10, i + 1)) - fmodl(n, powl(10, i)) * .9999) / powl(10, i);
  int digit = (int)(digit_double + 0.000001);
  if (digit != 0) {
    i++;
  }
  return i;
}

int round_fraction(double *fraction, int len) {
  *fraction *= pow(10, len);
  *fraction = round(*fraction);
  return (*fraction >= pow(10, len)) ? 1 : 0;
}

void integer_print(char **cursor, double integer) {
  int digit_c = digit_counter(integer);
  if (digit_c > -1) {
    for (int i = digit_c; i > 0; i--) {
      long double digit_double = (fmodl(integer, powl(10, i)) -
                                  fmodl(integer, powl(10, i - 1)) * .9999) /
                                 powl(10, i - 1);
      int digit = (int)(digit_double + 0.000001);
      one_digit_int_to_str(cursor, digit, '\0');
    }
  } else {
    **cursor = '0';
    *cursor += 1;
  }
}

void fraction_print(char **cursor, double fraction, int len) {
  for (int i = len; i > 0; i--) {
    long double digit_double = (fmodl(fraction, powl(10, i)) -
                                fmodl(fraction, powl(10, i - 1)) * .9999) /
                               powl(10, i - 1);
    int digit = (int)(digit_double + 0.000001);
    one_digit_int_to_str(cursor, digit, '\0');
  }
}

// int break_double_apart_e_form(double n, double *fraction, int power, int len)
// {
//   *fraction = 0.;
//   int main_digit = 0;
//   for (int i = power; i > power - len - 2; i--) {
//     long double digit_double, d1, d2;
//     d1 = fmodl(n, powl(10L, i));
//     d2 = fmodl(n * (1 + pow(10, i - 5)), powl(10, i - 1));
//     digit_double = (d1 - d2) / powl(10, i - 1);
//     int digit = (int)(digit_double + 0.000001);
//     if (i == power) {
//       main_digit = digit;
//     } else {
//       *fraction += digit * pow(10, i - power);
//     }
//   }
//   main_digit += round_fraction(fraction, len);
//   return main_digit;
// }

int max_pow(int x) {
  int log = 0;
  unsigned long long n = -1;
  for (; n >= 1; log++, n = n / x)
    ;
  return log;
}

void str_to_flags(char *str, int len, int *flags) {
  *flags = 0;
  for (int i = 0; i < len; i++) {
    switch (str[i]) {
      case '-':
        *flags = *flags | MINUS;
        break;
      case '+':
        *flags = *flags | PLUS;
        break;
      case ' ':
        *flags = *flags | SPASE;
        break;
      // case '#':
      //   *flags = *flags | HASH;
      //   break;
      case '0':
        *flags = *flags | ZERO;
        break;
    }
  }
}

s21_size_t s21_strspn(const char *str1, const char *str2) {
  s21_size_t len = 0;
  int flag = 1;
  for (int i = 0; str1[i] && flag; i++) {
    int bool = 1;
    for (int j = 0; str2[j] && bool; j++) {
      if (str1[i] == str2[j]) bool = 0;
    }
    if (bool == 0)
      len++;
    else
      flag = 0;
  }
  return len;
}