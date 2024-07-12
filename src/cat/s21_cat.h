#ifndef C3_SIMPLEBASHUTILS_1_SRC_S21_CAT_H_
#define C3_SIMPLEBASHUTILS_1_SRC_S21_CAT_H_

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct arguments {
  int b;
  int n;
  int s;
  int E;
  int T;
  int v;
} arguments;

arguments argument_parser(int, char**);
void output(arguments*, char**, int);
char v_output(char);
void outline(arguments*, char*, int);

#endif  // C3_SIMPLEBASHUTILS_1_SRC_S21_CAT_H_