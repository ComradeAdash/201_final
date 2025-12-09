#include <stdio.h>
#include <unistd.h>

int main() {
  char *str = "hello\n";
  FILE *f = fopen("tmp", "w+");
  fprintf(f, "%s", str);
  fflush(f);
  while (1) {
    sleep(30);
  }
}