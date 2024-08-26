#include <stdio.h>
#include <stdlib.h>

int main() {
  FILE *t1, *t2;

  t1 = fopen("/dev/pts/1", "r+");
  t2 = fopen("/dev/pts/3", "r+");

  fprintf(t1, "\nfrom t1");
  fprintf(t2, "\nfrom t2");

  return 0;
}
