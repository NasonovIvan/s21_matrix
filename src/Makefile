CC = gcc
FLAG_C = -c
FLAG_O = -o
# ASAN = -g -fsanitize=address
FLAG_COV = --coverage
FLAG_ER = -Wall -Werror -Wextra
s21_MATRIX_C = s21_*.c 
s21_MATRIX_O = s21_*.o
SUITE_CASES_C = suite_*.c
SUITE_CASES_O = suite_*.o

all: clean gcov_report

s21_matrix.a:
	$(CC) $(FLAG_C) $(FLAG_ER) $(s21_MATRIX_C)
	ar rc s21_matrix.a $(s21_MATRIX_O)
	ranlib s21_matrix.a

test: s21_matrix.a
	$(CC) -c test.c -o test.o
	$(CC) $(FLAG_ER) test.o s21_matrix.o -o tests -lcheck
	$(CC) $(FLAG_ER) $(FLAG_COV) $(FLAG_O) tests s21_matrix.a $(s21_MATRIX_C) test.o -lcheck
	./tests

gcov_report: test
	gcov s21_*.gcda	
	gcovr -b 
	gcovr
	gcovr --html-details -o report.html

check:
	cppcheck *.h *.c
	cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../materials/linters/cpplint.py --extension=c *.c *.h
	CK_FORK=no leaks --atExit -- ./tests
# valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./tests

clean:
	-rm -rf *.o *.html *.gcda *.gcno *.css *.a *.gcov *.info *.out *.cfg *.txt a.out.dSYM tests