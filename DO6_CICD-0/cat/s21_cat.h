#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_
#include <getopt.h>
#include <stdio.h>
#include <string.h>
char* short_opt = "beEnstTv";
typedef struct option option;
option long_opt[] = {{"number-nonblank", no_argument, NULL, 'b'},
                     {"number", no_argument, NULL, 'n'},
                     {"squeeze-blank", no_argument, NULL, 's'},
                     {NULL, no_argument, NULL, 0}};
typedef struct s21_cat {
  int b, e, E, n, s, t, T, v;
} cat_flags;
int s21_parser(cat_flags* f, int argc, char* const* argv, const char* short_opt,
               option* long_opt);
void s21_check_flags(cat_flags* f);
void s21_write_to_file(FILE* fp, cat_flags* f, int* begin_row, int* end_row,
                       int* row, int* squeeze);
#endif  // SRC_CAT_S21_CAT_H_
