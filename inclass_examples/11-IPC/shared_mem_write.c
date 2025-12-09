#define _XOPEN_SOURCE 500

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s /shm-path\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *shmpath = argv[1];

  int fd = shm_open(shmpath, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }

  if (ftruncate(fd, sizeof(uint8_t)) == -1) {
    perror("ftruncate");
    exit(EXIT_FAILURE);
  }

  /* Map the object into the caller's address space */

  uint8_t *ptr =
      mmap(NULL, sizeof(*ptr), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  *ptr = 100;
}