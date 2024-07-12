CFLAGS = -Wall -Werror -Wextra
s21_grep: s21_grep.o
	gcc $(CFLAGS) *.o -o s21_grep 
s21_grep.o: s21_grep.c 
	gcc $(FLAGS) -c s21_grep.c
test:
	bash test_func_grep.sh
clean:
	rm *.o