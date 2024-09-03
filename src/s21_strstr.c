#include "s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  char *result = s21_NULL;  // Указатель на начало найденной подстроки или NULL

  if (!*needle)  // Если needle пустая строка, возвращаем haystack.
    return (char *)haystack;

  char *haystack_current;
  char *needle_current;
  char *start =
      (char *)haystack;  // Начальная точка каждой новой проверки в haystack.

  while (*start &&
         !result)  // Пока не достигнут конец haystack и не найдена подстрока.
  {
    haystack_current = start;
    needle_current = (char *)needle;
    while (*haystack_current && *needle_current &&
           *haystack_current ==
               *needle_current)  // Проверяем совпадение символов.
    {
      haystack_current++;
      needle_current++;
    }
    if (!*needle_current)  // Если needle_current достигло конца needle, значит
                           // подстрока найдена.
    {
      result = start;
    }
    start++;  // Перемещаем начальную точку поиска.
  }

  return result;  // Возвращаем найденную подстроку или NULL, если она не
                  // найдена.
}
