CFLAGS = -Wall -Werror -Wextra
s21_cat: s21_cat.o
	gcc $(CFLAGS) s21_cat.o -o s21_cat
test:
	bash tests.sh
s21_cat.o: s21_cat.c 
	gcc $(CFLAGS) -c s21_cat.c 
clean:
	rm *.o