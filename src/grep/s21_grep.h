#ifndef C3_SIMPLEBASHUTILS_1_SRC_S21_GREP_H_
#define C3_SIMPLEBASHUTILS_1_SRC_S21_GREP_H_

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct print {
  char *line;
  char *filepath;
  int status;
  int line_status;
  int line_count;
  int current_line;
  int file_write_count;
} printf_info;

typedef struct opt {
  int e;
  char **e_temp;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  char **f_temp;
  int o;
  int pattern_count;
  char **pattern;
  int file_count;
} opt;

void writefile();
void line_prep(char *line);
int word_length(char *line);
int file_patterns(opt *options);
int non_pattern_flags_count(opt *options);
void retype(int number, int *type, opt *options);
void flagO(FILE **fin, opt *options, char *filepath);
void grep_parser(FILE **fin, opt *options, char *filepath);
int parser(int argc, char **argv, int *type, opt *options);
void flags_parser(FILE **fin, opt *options, char *filepath);
int pattern_prep(int argc, char **argv, int type[], opt *options);
int options_parser(int number, char **argv, int *type, opt *options);
char *printO(FILE **dst, opt *options, printf_info *info, char *string,
             char *pattern);
void flags_operation(FILE **dst, opt options, char *filepath,
                     printf_info *info);
int flagE_parser(size_t point, int number, char **argv, int *type,
                 opt *options);
int flagF_parser(size_t point, int number, char **argv, int *type,
                 opt *options);
int match(char *string, char *pattern, regex_t *re, opt *options);

#endif  // C3_SIMPLEBASHUTILS_1_SRC_S21_GREP_H_
