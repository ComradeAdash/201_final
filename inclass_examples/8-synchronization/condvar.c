#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static int avail = 0;

static void *thread_func(void *arg) {
  for (;;) {
    int s = pthread_mutex_lock(&mtx);
    if (s != 0) {
      perror("pthread_mutex_lock");
      pthread_exit((void *)1);
    }
    avail++;

    s = pthread_mutex_unlock(&mtx);
    if (s != 0) {
      perror("pthread_mutex_unlock");
      pthread_exit((void *)1);
    }
    sleep(1);
  }

  return 0;
}

int main() {
  pthread_t t1;
  void *res;
  int s;

  s = pthread_create(&t1, NULL, thread_func, NULL);
  if (s != 0) {
    perror("pthread_create");
    exit(1);
  }

  for (;;) {
    s = pthread_mutex_lock(&mtx);
    if (s != 0) {
      perror("pthread_mutex_lock");
      exit(1);
    }

    while (avail > 0) {
      /* This is simulating "consume everything available" */
      avail--;
    }

    s = pthread_mutex_unlock(&mtx);
    if (s != 0) {
      perror("pthread_mutex_unlock");
      exit(1);
    }
  }

  s = pthread_join(t1, &res);
  if (s != 0) {
    perror("pthread_create");
    exit(1);
  }
}