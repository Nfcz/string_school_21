SHELL:= /bin/bash

COMP = gcc -g
FLAGS = -Werror -Wextra -Wall -std=c11
FLAGS_C = $(FLAGS) -c
FILE_SOURSE = *.c
TEST_FILE_SOURSE = test/test_s21_string.check
TEST_FILE_C = test/test_s21_string.c
TEST_FILE_OBJ = test/test_s21_string.o
lib_string_flag = -L. -l:s21_string.a

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)
  TEST_LIB = -lcheck -lm
else
#sudo apt-get update
#sudo apt-get install pkg-config
  TEST_LIB = $(shell pkg-config --cflags --libs check)
endif




all: clean s21_string.a gcov_report

test: s21_string.a $(TEST_FILE_OBJ)
	$(COMP) $(TEST_FILE_OBJ) -o a.out $(lib_string_flag) $(TEST_LIB) -lm
	@-./a.out

$(TEST_FILE_OBJ): $(TEST_FILE_C)
	@$(COMP) $(FLAGS_C) -Wno-stringop-truncation -Wno-memset-transposed-args $(TEST_FILE_C) -o $(TEST_FILE_OBJ) -lm

$(TEST_FILE_C):
	@checkmk $(TEST_FILE_SOURSE) > $(TEST_FILE_C)

s21_string.a:
	@$(COMP) $(FLAGS_C) -c $(FILE_SOURSE) -lm
	@ar rcs s21_string.a *.o
	@ranlib s21_string.a
	@rm -rf *.o

gcov_report: $(FILE_SOURSE) $(TEST_FILE_OBJ)
	@$(COMP) $(FLAGS) -Wno-stringop-truncation -Wno-memset-transposed-args -fprofile-arcs -ftest-coverage $(TEST_FILE_OBJ) $(FILE_SOURSE) -o gcov_report $(TEST_LIB)
	@./gcov_report
	@lcov --directory . -t "gcov_report" -o gcov_report.info -c
	@genhtml -o report gcov_report.info
	@-rm -f *.gcno *.gcda gcov_report.info
	@-open report/index.html

clean:
	@-rm -rf *.o */*.o $(TEST_FILE_C) gcov_report a.out report *.gcno *.gcda gcov_report.dSYM *.a

clean_for_linux:

	@-rm -rf *.o */*.o a.out *.a s21_string.a

rebuild: clean s21_string.a

leaks: test
	@leaks -atExit -- ./a.out

valgrind: test
	@valgrind -s --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./a.out
# --tool=memcheck 
cl:
	@-clang-format -i *.c *.h */*.c */*.h