#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  char *str = "hello\n";
  int fd = open("tmp", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  write(fd, str, strlen(str));
  while (1) {
    sleep(30);
  }
}