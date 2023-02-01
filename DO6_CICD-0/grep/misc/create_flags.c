#include <stdio.h>
#define NUM_FLAGS 8

void comb(int from, int to, unsigned long chosen, int at, FILE *fp,
          char **flags) {
  unsigned long one = 1;
  if (to >= from + at) {
    if (from != 0) {
      comb(from - 1, to, chosen | (one << at), at + 1, fp, flags);
      comb(from, to, chosen, at + 1, fp, flags);
    } else {
      for (int i = 0; i < to; i++) {
        if (chosen & (one << i)) {
          fprintf(fp, "-%s ", flags[i]);
        }
      }
      if (chosen != (1UL << (NUM_FLAGS)) - 1) {
        fprintf(fp, ":");
      }
    }
  }
}

void write_flags(char *path, char **flags, int num) {
  FILE *fp;
  fp = fopen(path, "w+");
  fprintf(fp, ":");
  for (int i = 1; i <= num; i++) {
    comb(i, num, 0UL, 0, fp, flags);
  }
  fclose(fp);
}

int main() {
  char *flags[] = {"i", "v", "c", "l", "n", "h", "s", "o"};
  char *path = "flags/flags.txt";
  write_flags(path, flags, NUM_FLAGS);
  return 0;
}
