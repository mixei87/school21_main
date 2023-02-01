#include <stdio.h>

int main() {
  FILE *fp;
  fp = fopen("ascii.txt", "w");
  for (unsigned int i = 0; i <= 255; i++) {
    fprintf(fp, "%d\t%lc\n", i, i);
  }
  fclose(fp);
  return 0;
}