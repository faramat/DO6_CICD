#include "s21_grep.h"

int main(int argc, char *argv[]) {
  opt options = {0};
  int *type = calloc(argc, sizeof(int));
  int error = 0;
  error += parser(argc, argv, type, &options);
  error += pattern_prep(argc, argv, type, &options);

  for (int i = 0; i < argc; i++) {
    if (type[i] == 5) options.file_count++;
  }
  if (!error) {
    for (int i = 0; i < argc; i++) {
      if (type[i] == 5) {
        FILE *fin = fopen(argv[i], "r");
        if (fin != NULL) {
          grep_parser(&fin, &options, argv[i]);
          fclose(fin);
          writefile();
        }
      }
    }
  }
  free(type);
  for (int i = 0; i < options.e; i++) free(options.e_temp[i]);
  for (int i = 0; i < options.f; i++) free(options.f_temp[i]);
  for (int i = 0; i < options.pattern_count; i++) free(options.pattern[i]);
  if (options.pattern) free(options.pattern);
  if (options.e_temp) free(options.e_temp);
  if (options.f_temp) free(options.f_temp);

  return 0;
}

void writefile() {
  FILE *fin = fopen("buffer.txt", "r");
  char *line = NULL;
  size_t len = 0;
  int read;
  while ((read = getline(&line, &len, fin)) != -1) {
    printf("%s", line);
  }

  if (line) free(line);

  fclose(fin);
  remove("buffer.txt");
}

int parser(int argc, char *argv[], int *type, opt *options) {
  int error = 0;
  for (int i = 1; i < argc; i++) {
    if (!type[i]) {
      if (argv[i][0] == '-') {
        type[i] = 1;
        error = options_parser(i, argv, type, options);
      } else if (!options->pattern_count && !options->e && !options->f) {
        type[i] = 2;
        options->pattern_count++;
      } else {
        type[i] = 5;
      }
    }
  }
  return error;
}

int options_parser(int number, char **argv, int *type, opt *options) {
  int error = 0;
  int state = 0;
  for (size_t i = 1; i < strlen(argv[number]) && !state; i++) {
    if (argv[number][i] == 'e') {
      state = flagE_parser(i, number, argv, type, options);
    } else if (argv[number][i] == 'f') {
      state = flagF_parser(i, number, argv, type, options);
    } else if (argv[number][i] == 'i') {
      options->i = 1;
    } else if (argv[number][i] == 'v') {
      options->v = 1;
    } else if (argv[number][i] == 'c') {
      options->c = 1;
    } else if (argv[number][i] == 'l') {
      options->l = 1;
    } else if (argv[number][i] == 'n') {
      options->n = 1;
    } else if (argv[number][i] == 'h') {
      options->h = 1;
    } else if (argv[number][i] == 's') {
      options->s = 1;
    } else if (argv[number][i] == 'o') {
      options->o = 1;
    } else {
      fprintf(stderr, "grep: invalid option -- %c", argv[number][i]);
      error = 1;
    }
  }
  return error;
}

int flagE_parser(size_t point, int number, char **argv, int *type,
                 opt *options) {
  int state = 0;
  options->e++;
  retype(number, type, options);
  options->e_temp = realloc(options->e_temp, sizeof(char *) * options->e);
  if (point == strlen(argv[number]) - 1) {
    options->e_temp[(options->e) - 1] =
        calloc(strlen(argv[number + 1]) + 1, sizeof(char));
    strcat(options->e_temp[(options->e) - 1], argv[number + 1]);
    type[number + 1] = 3;
    state = 1;
  } else {
    state = 1;
    char *e_pattern = calloc(strlen(argv[number]), sizeof(char));
    type[number] = 13;
    int point = 0;
    for (size_t j = point + 2; j < strlen(argv[number]); j++)
      e_pattern[point++] = argv[number][j];
    options->e_temp[(options->e) - 1] = e_pattern;
  }
  return state;
}

int flagF_parser(size_t point, int number, char **argv, int *type,
                 opt *options) {
  int state = 0;
  options->f++;
  retype(number, type, options);
  options->f_temp = realloc(options->f_temp, sizeof(char *) * options->f);
  if (point == strlen(argv[number]) - 1) {
    options->f_temp[(options->f) - 1] =
        calloc(strlen(argv[number + 1]) + 1, sizeof(char));
    strcat(options->f_temp[(options->f) - 1], argv[number + 1]);
    type[number + 1] = 4;
    state = 1;
  } else {
    state = 1;
    char *f_pattern = calloc(strlen(argv[number]), sizeof(char));
    type[number] = 14;
    int point = 0;
    for (size_t j = point + 2; j < strlen(argv[number]); j++)
      f_pattern[point++] = argv[number][j];
    options->f_temp[(options->f) - 1] = f_pattern;
  }
  return state;
}

void retype(int number, int *type, opt *options) {
  for (int i = 0; i < number; i++) {
    if (type[i] == 2) {
      options->pattern_count--;
      type[i] = 5;
    }
  }
}

int pattern_prep(int argc, char **argv, int type[], opt *options) {
  int error = 0;
  for (int i = 0; i < argc; i++) {
    if (type[i] == 2) {
      options->pattern = calloc(1, sizeof(char *));
      options->pattern[0] = calloc(strlen(argv[i]) + 1, sizeof(char));
      strcat(options->pattern[0], argv[i]);
    }
  }
  if (options->e) {
    options->pattern_count += options->e;
    options->pattern = realloc(options->pattern, sizeof(char *) * options->e);
    for (int i = 0; i < options->e; i++) {
      options->pattern[i] =
          calloc(strlen(options->e_temp[i]) + 1, sizeof(char));
      strcat(options->pattern[i], options->e_temp[i]);
    }
  }
  if (options->f) {
    error = file_patterns(options);
  }
  return error;
}

int file_patterns(opt *options) {
  int error = 0;
  int line_count = 0;
  char *line = NULL;
  size_t len = 0;
  int read;
  for (int i = 0; i < options->f; i++) {
    FILE *fin = fopen(options->f_temp[i], "r");
    if (fin != NULL) {
      while ((read = getline(&line, &len, fin)) != -1) {
        options->pattern = realloc(
            options->pattern, sizeof(char *) * (options->pattern_count + 1));
        options->pattern[options->e + line_count] =
            calloc(word_length(line) + 1, sizeof(char));
        strncat(options->pattern[options->e + line_count], line,
                word_length(line));
        line_count++;
        options->pattern_count = options->e + line_count;
      }
      fclose(fin);
    } else {
      fprintf(stderr, "grep: %s: No such file or directory\n",
              options->f_temp[i]);
    }
  }
  if (line) free(line);

  return error;
}

int word_length(char *line) {
  int length = 0;
  while (line[length] != '\0' && line[length] != '\n') {
    length++;
  }
  return length;
}

void grep_parser(FILE **fin, opt *options, char *filepath) {
  if (options->o && !options->v && !options->c && !options->l) {
    flagO(fin, options, filepath);
  } else {
    flags_parser(fin, options, filepath);
  }
}

void flagO(FILE **fin, opt *options, char *filepath) {
  FILE *dst = fopen("buffer.txt", "w");
  char *line = NULL;
  printf_info info;
  size_t len = 0;
  int read;
  info.current_line = 0;
  info.file_write_count = 1;
  while ((read = getline(&line, &len, *fin)) != -1) {
    info.status = 0;
    info.current_line++;
    line[word_length(line)] = '\0';
    info.line = line;
    info.filepath = filepath;
    info.line_status = 1;
    for (int i = 0; i < options->pattern_count; i++) {
      info.line = printO(&dst, options, &info, info.line, options->pattern[i]);
    }
  }
  if (line) free(line);
  fclose(dst);
}

char *printO(FILE **dst, opt *options, printf_info *info, char *string,
             char *pattern) {
  regmatch_t pmatch[1];
  regex_t regex;
  int status = 0;
  if (options->i)
    status = regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE);
  else
    status = regcomp(&regex, pattern, REG_EXTENDED);

  if (!status) {
    size_t nmatch = 1;
    while (regexec(&regex, string, nmatch, pmatch, 0) == 0) {
      if (info->file_write_count || info->line_status) {
        info->file_write_count = 0;
        if ((options->l || options->file_count > 1) && !options->h)
          fprintf(*dst, "%s:", info->filepath);
        if (options->n && info->line_status) {
          fprintf(*dst, "%d:", info->current_line);
        }

      } else if (!options->l && options->file_count > 1 && info->line_status &&
                 !options->h) {
        fprintf(*dst, "%s:", info->filepath);
        info->line_status = 0;
      }

      for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
        fprintf(*dst, "%c", string[i]);
      }
      fprintf(*dst, "\n");
      string = string + pmatch[0].rm_eo;
    }
  }
  regfree(&regex);
  return string;
}

void flags_parser(FILE **fin, opt *options, char *filepath) {
  FILE *dst = fopen("buffer.txt", "w");
  char *line = NULL;
  printf_info info;
  size_t len = 0;
  int read, retval, error = 1;
  info.current_line = 0;
  info.line_count = 0;
  info.file_write_count = 0;
  regex_t re;
  while ((read = getline(&line, &len, *fin)) != -1) {
    info.status = 0;
    info.current_line++;
    line[word_length(line)] = '\0';
    info.line = line;
    int line_concur_count = 0;
    int break_v = 0;
    for (int i = 0; i < options->pattern_count && !break_v; i++) {
      retval = match(line, options->pattern[i], &re, options);
      if (options->v && retval) {
        info.status = 0;
        break_v = 1;
        line_concur_count = 0;
      }
      if (retval == 0) {
        info.status++;
        line_concur_count++;
      }
      regfree(&re);
    }

    if (line_concur_count) info.line_count++;

    if (info.status && !options->c) {
      flags_operation(&dst, *options, filepath, &info);
      info.file_write_count++;
    }
  }
  if (options->c && options->l && info.line_count == 0) {
    error = 0;
  }

  if (options->c && !info.file_write_count && error) {
    flags_operation(&dst, *options, filepath, &info);
  }
  if (line) free(line);
  fclose(dst);
}

void flags_operation(FILE **dst, opt options, char *filepath,
                     printf_info *info) {
  int options_count = non_pattern_flags_count(&options);
  int pos_1 = 0;
  int pos_2 = 0;
  int printed = 0;

  if (options.l && !options.c && !info->file_write_count) {
    fprintf(*dst, "%s", filepath);
    printed++;
    pos_1 = -1;
  }
  if (options.l && options.c) {
    fprintf(*dst, "%s", filepath);
    printed++;
    pos_1 = 1;
  } else if (options.file_count > 1 && !options.l && !options.h) {
    fprintf(*dst, "%s", filepath);
    printed++;
    pos_1 = 2;
  }
  if ((pos_1 != -1 && pos_1 != 0 && options_count > 1) ||
      (pos_1 == 2 && info->status)) {
    printed++;
    fprintf(*dst, ":");
  }
  if (options.c && (options.l && info->line_count > 0)) {
    pos_2 = -1;
    fprintf(*dst, "%d", info->line_count);
    printed++;
  } else if (options.c) {
    fprintf(*dst, "%d", info->line_count);
    printed++;
    pos_2 = 1;
  } else if (options.n && !options.c && !options.l) {
    fprintf(*dst, "%d", info->current_line);
    printed++;
    pos_2 = 2;
  }
  if (pos_2 != 0 && !options.c && !options.l) fprintf(*dst, ":");
  if (!options.c && !options.l) {
    fprintf(*dst, "%s", info->line);
    printed++;
  }
  if (printed) fprintf(*dst, "\n");
}

int non_pattern_flags_count(opt *options) {
  int result = 0;
  result += options->h;
  result += (options->l || options->file_count > 1);
  result += options->c;
  result += options->n;
  return result;
}

int match(char *string, char *pattern, regex_t *re, opt *options) {
  int status = -1;
  if (!options->i && (status = regcomp(re, pattern, REG_EXTENDED) == 0)) {
    status = regexec(re, string, 0, NULL, 0);
  } else if ((options->i) &&
             (status = regcomp(re, pattern, REG_EXTENDED | REG_ICASE)) == 0) {
    status = regexec(re, string, 0, NULL, 0);
  }

  if (options->v) status = !status;

  return (status);
}
