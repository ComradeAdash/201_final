#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cnt = 0;

static void *thread_func(void *arg) {
  for (int i = 0; i < 10000000; i++)
    cnt++;
  pthread_exit(0);
}

int main(int argc, char *argv[]) {
  pthread_t t1;
  pthread_t t2;

  if (pthread_create(&t1, NULL, thread_func, NULL) != 0)
    perror("pthread_create");

  if (pthread_create(&t2, NULL, thread_func, NULL) != 0)
    perror("pthread_create");

  if (pthread_join(t1, NULL) != 0)
    perror("pthread_join");
  if (pthread_join(t2, NULL) != 0)
    perror("pthread_join");

  printf("%d\n", cnt);

  exit(EXIT_SUCCESS);
}