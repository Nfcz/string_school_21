#ifndef S21_SPRINTF
#define S21_SPRINTF

#include <math.h>

#include "s21_string.h"
typedef struct specifier {
  char sp, l, *start, *end;
  int w, pre, error, fl;
} spec;

spec parser(const char *format);
void width_parser(spec *spec, char **start);
void precision_parser(spec *spec, char **start);
void specifier_to_str(spec spec, char *start, char **cursor, va_list *input);
void spec_preprocessing(spec *spec);
void star_processing(spec *spec, va_list *input);
void s_to_str(spec spec, char **cursor, va_list *input);
void d_to_str(spec spec, char **cursor, va_list *input);
void u_to_str(spec spec, char **cursor, va_list *input, int counting_system,
              char flag);
void p_to_str(spec spec, char **cursor, va_list *input);
void width_and_minus_to_str(spec spec, char **cursor, char *start_print_spec);
int one_digit_str_to_int(char cursor);
int str_to_int_parser(char *str, int *res, int len);
void one_digit_int_to_str(char **cursor, int n, char flag);
void int_to_str_10(char **cursor, long int n, int flag, int pre);
void unsigned_int_to_str(char **cursor, unsigned long long int n,
                         int counting_system, char flag, int pre);
void float_to_str(char **cursor, double n, int len, int flag);
void float_to_mantissa_str(char **cursor, double n, int len, char flag);
int digit_counter(double n);
int round_fraction(double *fraction, int len);
void integer_print(char **cursor, double integer);
void fraction_print(char **cursor, double fraction, int len);
int break_double_apart_e_form(double n, double *fraction, int power, int len);
int max_pow(int x);
void str_to_flags(char *str, int len, int *flags);
s21_size_t s21_strspn(const char *str1, const char *str2);

#endif