#include "s21_cat.h"

int main(int argc, char* argv[]) {
  cat_flags f = {0};

  int error = s21_parser(&f, argc, argv, short_opt, long_opt);
  if (error == 0) {
    s21_check_flags(&f);

#if defined(__linux__)
    int begin_row = 1, end_row = 1, squeeze = 1, row = 1;
#endif
    for (int i = 1; i < argc; i++) {
      FILE* fp;
      fp = fopen(argv[i], "r+");
      if (fp != NULL) {
#if defined(__APPLE__)
        int begin_row = 1, end_row = 1, row = 1, squeeze = 1;
#endif
        s21_write_to_file(fp, &f, &begin_row, &end_row, &row, &squeeze);
        fclose(fp);
      }
    }
  }
  return 0;
}

void s21_write_to_file(FILE* fp, cat_flags* f, int* begin_row, int* end_row,
                       int* row, int* squeeze) {
  int ch;
  while ((ch = fgetc(fp)) != EOF) {
    if (f->s == 1) {
      if (ch == '\n') {
        if (*squeeze < 2) {
          *squeeze = *squeeze + 1;
        } else {
          continue;
        }
      } else {
        *squeeze = 0;
      }
    }
    if (f->n == 1) {
      if (*begin_row == 1) {
        printf("%6d\t", *row);
        *begin_row = 0;
      }
      if (ch == '\n') {
        *begin_row = 1, *row = *row + 1;
      }
    }
    if (f->b == 1) {
      if (ch != '\n') {
        *begin_row = 1;
      } else {
        *begin_row = 0, *end_row = 1;
      }
      if (*begin_row == 1 && *end_row == 1) {
        printf("%6d\t", *row);
        *end_row = 0, *row = *row + 1;
      }
    }
    if (f->v == 1) {
      if ((ch >= 0 && ch <= 8) || (ch >= 11 && ch <= 31)) {
        printf("^");
        ch += 0x40;
      } else if (ch == 127) {
        printf("^");
        ch = 0x3F;
      } else if (ch >= 128 && ch <= 159) {
        printf("M-^");
        ch -= 0x40;
      } else if (ch >= 160 && ch <= 254) {
#if defined(__linux__)
        printf("M-");
        ch -= 0x80;
      } else if (ch == 255) {
        printf("M-^");
        ch = 0x3F;
#endif
      }
    }
    if (ch == 9 && f->t == 1) {
      printf("^");
      ch = 'I';
    }
    if (ch == 10 && f->e == 1) {
      printf("$");
    }
    printf("%c", ch);
  }
}

void s21_check_flags(cat_flags* f) {
  if (f->b == 1) {
    f->n = 0;
  }
  if (f->e == 1 || f->t == 1) {
    f->v = 1;
  }
  if (f->E == 1) {
    f->e = 1;
  }
  if (f->T == 1) {
    f->t = 1;
  }
}

int s21_parser(cat_flags* f, int argc, char* const* argv, const char* short_opt,
               option* long_opt) {
  int res, opt_index, error = 0;
  while (((res = getopt_long(argc, argv, short_opt, long_opt, &opt_index)) !=
          -1) &&
         (error == 0)) {
    if (res == '?')
      error = 1;
    else if (res == 'b')
      f->b = 1;
    else if (res == 'e')
      f->e = 1;
    else if (res == 'E')
      f->E = 1;
    else if (res == 'n')
      f->n = 1;
    else if (res == 's')
      f->s = 1;
    else if (res == 't')
      f->t = 1;
    else if (res == 'T')
      f->T = 1;
    else if (res == 'v')
      f->v = 1;
  }
  return error;
}
