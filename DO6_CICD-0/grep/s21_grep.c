#include "s21_grep.h"
int main(int argc, char* argv[]) {
  grep_flags f = {0};
  extern int optind, opterr, optopt;
  extern char* optarg;
  grep_strings p[2];  // p[0] - patterns, p[1] - files
  for (int i = 0; i < 2; i++) {
    s21_malloc_grep_str(&p[i], 40);
  }
  s21_parser(&f, argc, argv, optstring, p);
  s21_check_flags(&f);
  char* ptr = strtok(p[1].str, "|");

  while (ptr != NULL) {
    FILE* fp;
    fp = fopen(ptr, "r+");
    if (fp != NULL) {
      s21_write_to_stdin(fp, p, &f, ptr);
      fclose(fp);
    } else if (f.s == 0) {
      fprintf(stderr, "grep: %s: No such file or directory\n", ptr);
    }
    ptr = strtok(NULL, "|");
  }

  for (int i = 0; i < 2; i++) {
    s21_free_grep_str(&p[i]);
  }
  return 0;
}

void s21_parser(grep_flags* f, int argc, char* const* argv,
                const char* optstring, grep_strings p[]) {
  grep_strings tmp_string;
  s21_malloc_grep_str(&tmp_string, 70);
#if defined(__APPLE__)
  for (; optind < argc; optind++) {
#endif
    int res;
    while ((res = getopt(argc, argv, optstring)) != -1) {
      if (res == 'e') {
        f->e = 1;
        s21_write_to_string(&p[0], optarg);
      } else if (res == 'f') {
        f->f = 1;
        s21_handler_f(p, optarg);
      } else if (res == 'i') {
        f->i = 1;
      } else if (res == 'v') {
        f->v = 1;
      } else if (res == 'c') {
        f->c = 1;
      } else if (res == 'l') {
        f->l = 1;
      } else if (res == 'n') {
        f->n = 1;
      } else if (res == 'h') {
        f->h = 1;
      } else if (res == 's') {
        f->s = 1;
      } else if (res == 'o') {
        f->o = 1;
      } else if (res == 1) {
#if defined(__linux__)
        if (f->e == 1 || f->f == 1) {
          s21_write_to_string(&p[1], argv[optind - 1]);
        } else {
          s21_write_to_string(&tmp_string, argv[optind - 1]);
        }
#endif
      }
#if defined(__APPLE__)
    }
    if (optind < argc) {
      if (f->e == 1 || f->f == 1) {
        s21_write_to_string(&p[1], argv[optind]);
      } else {
        s21_write_to_string(&tmp_string, argv[optind]);
      }

#endif
    }
#if defined(__APPLE__)
  }
#endif
#if defined(__linux__)
  for (; optind < argc; optind++) {
    if (f->e == 1 || f->f == 1) {
      s21_write_to_string(&p[1], argv[optind - 1]);
    } else {
      s21_write_to_string(&tmp_string, argv[optind - 1]);
    }
  }
#endif
  if (tmp_string.num_substring > 0) {
    char* pos;
    p[1].num_substring = tmp_string.num_substring - 1;
    if (f->e == 1 || f->f == 1) {
      s21_write_to_string(&p[1], tmp_string.str);
    } else if (tmp_string.num_substring > 1) {
      pos = strtok(tmp_string.str, "|");
      s21_write_to_string(&p[0], pos);
      p[1].num_substring--;
      pos = strchr(tmp_string.str, 0) + 1;
      s21_write_to_string(&p[1], pos);
    }
  }
  s21_free_grep_str(&tmp_string);
}

void s21_write_to_stdin(FILE* fp, grep_strings* p, grep_flags* f,
                        char* name_file) {
  regex_t expr;
  regmatch_t pmatch[2];
  int flags_regcomp = REG_EXTENDED;
  if (f->i == 1) flags_regcomp |= REG_ICASE;
  int status = regcomp(&expr, p[0].str, flags_regcomp);
  if (status == 0) {
    int ch = fgetc(fp), flag_c = 0, row = 1;
    while (ch != EOF) {
      grep_strings str;
      s21_malloc_grep_str(&str, 200);
      while (ch != '\n' && ch != EOF) {
        s21_realloc_grep_str(&str, 1);
        char x[2] = {0};
        x[0] = ch;
        memmove(&str.str[str.cur_pos], x, 2);
        str.cur_pos++;
        ch = fgetc(fp);
      }
      int nmatch = 2;
      if (f->o == 0)
        status = regexec(&expr, str.str, 0, NULL, 0);
      else
        status = regexec(&expr, str.str, nmatch, pmatch, 0);
      if ((status == 0 && f->v == 0) || (status == 1 && f->v == 1)) {
        if (f->h == 0 && f->l == 0 && p[1].num_substring > 1 && f->c == 0) {
#if defined(__linux__)
          if (f->o == 0)
#endif
            printf("%s:", name_file);
        }
        if (f->n == 1 && f->c == 0 && f->l == 0) {
#if defined(__linux__)
          if (f->o == 0)
#endif
            printf("%d:", row);
        }
        if (f->l == 0 && f->c == 0 && f->o == 0) {
          printf("%s\n", str.str);
        }
        if (f->o == 1 && f->c == 0 && f->l == 0) {
          char* temp = str.str;
          while (status == 0) {
#if defined(__linux__)
            if (f->h == 0 && p[1].num_substring > 1) {
              printf("%s:", name_file);
            }
            if (f->n == 1) {
              printf("%d:", row);
            }
#endif
            printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                   &temp[pmatch[0].rm_so]);
            temp += pmatch[0].rm_eo;
            status = regexec(&expr, temp, nmatch, pmatch, 0);
          }
        }
        flag_c++;
      }
      s21_free_grep_str(&str);
      if (ch != EOF) {
        ch = fgetc(fp);
        row++;
      }
    }
    if (p[1].num_substring > 1 && f->c == 1 && f->h == 0) {
#if defined(__linux__)
      if (f->l == 0)
#endif
        printf("%s:", name_file);
    }
    if (f->l == 1) {
      flag_c = flag_c > 0 ? 1 : 0;
    }
#if defined(__APPLE__)
    if (f->c == 1) {
      printf("%d\n", flag_c);
    }
#elif defined(__linux__)
    if (f->c == 1 && f->l == 0) {
      printf("%d\n", flag_c);
    }
#endif
    if (f->l == 1 && flag_c > 0) {
      printf("%s\n", name_file);
    }
  }
  regfree(&expr);
}

void s21_malloc_grep_str(grep_strings* str, int size) {
  str->size = size;
  str->str = (char*)malloc(str->size + 1);
  str->str[0] = 0;
  str->cur_pos = 0;
  str->num_substring = 0;
}

void s21_realloc_grep_str(grep_strings* str, int len_new) {
  while ((str->cur_pos + len_new) > str->size) {
    s21_realloc_only_grep_str(str);
  }
}

void s21_realloc_only_grep_str(grep_strings* str) {
  str->size *= 2;
  str->str = realloc(str->str, str->size + 1);
}

void s21_free_grep_str(grep_strings* str) {
  free(str->str);
  str->str = NULL;
}

void s21_check_flags(grep_flags* f) {
#if defined(__APPLE__)
  if (f->v == 1) {
    f->o = 0;
  }
#endif
}

void s21_write_to_string(grep_strings* p, char* new_pattern) {
  if (p->cur_pos > 0) {
    s21_realloc_grep_str(p, 1);
    memmove(&(p->str[p->cur_pos]), "|\0", 2);
    p->cur_pos++;
  }
  if (new_pattern[0] == 0) {
    s21_realloc_grep_str(p, 2);
    memmove(&(p->str[p->cur_pos]), ".*\0", 3);
    p->cur_pos += 2;
  } else {
    int len_new = strlen(new_pattern);
    s21_realloc_grep_str(p, len_new);
    memmove(&(p->str[p->cur_pos]), new_pattern, len_new + 1);
    p->cur_pos += len_new;
  }
  p->num_substring++;
}

void s21_handler_f(grep_strings* p, char* file) {
  FILE* fp;
  fp = fopen(file, "r+");
  if (fp != NULL) {
    int ch, flag_N = 1, flag_blank = 1;
    grep_strings new_pattern;
    s21_malloc_grep_str(&new_pattern, 40);
    for (int i = 0; (ch = fgetc(fp)) != EOF; i++) {
      if (flag_blank == 1) {
        flag_blank = 0;
      }
      if (ch == '\n') {
        if (flag_N == 1) {
          s21_write_to_string(&p[0], "\0");
          break;
        } else {
          s21_write_to_string(&p[0], new_pattern.str);
          flag_N = 1;
        }
        s21_free_grep_str(&new_pattern);
        s21_malloc_grep_str(&new_pattern, 40);
        i = -1;
      } else {
        s21_realloc_grep_str(&new_pattern, 1);
        char x[2] = {0};
        x[0] = ch;
        memmove(&new_pattern.str[new_pattern.cur_pos], x, 2);
        new_pattern.cur_pos++;
        // sprintf(new_pattern.str, "%s%c", new_pattern.str, ch);
        if (flag_N == 1) {
          flag_N = 0;
        }
      }
    }
    if (flag_blank == 0 && flag_N == 0) {
      s21_write_to_string(&p[0], new_pattern.str);
    }
    if (new_pattern.str != NULL) {
      s21_free_grep_str(&new_pattern);
    }
    fclose(fp);
  }
}
