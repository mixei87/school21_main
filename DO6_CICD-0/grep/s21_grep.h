#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char* optstring = "-e:ivclnhsf:o";

typedef struct s21_grep {
  int e, i, v, c, l, n, h, s, o, f;
} grep_flags;
typedef struct s21_string {
  char* str;
  int size, cur_pos, num_substring;
} grep_strings;

typedef struct s21_matches {
  char** arr_str;
  int i, j, cur_pos;
} grep_matches;

void s21_write_to_stdin(FILE* fp, grep_strings* p, grep_flags* f,
                        char* name_file);
void s21_malloc_grep_str(grep_strings* str, int size);
void s21_realloc_grep_str(grep_strings* str, int len_new);
void s21_realloc_only_grep_str(grep_strings* str);
void s21_free_grep_str(grep_strings* str);
void s21_check_flags(grep_flags* f);
void s21_parser(grep_flags* f, int argc, char* const* argv,
                const char* optstring, grep_strings p[]);

void s21_write_to_string(grep_strings* p, char* new_pattern);
void s21_handler_f(grep_strings* p, char* file);

#endif  // SRC_GREP_S21_GREP_H_
