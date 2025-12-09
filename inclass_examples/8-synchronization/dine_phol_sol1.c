#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMBER 5

static pthread_mutex_t mtx[NUMBER] = {PTHREAD_MUTEX_INITIALIZER};

static void *thread_func(void *arg) {
  int left = (int)arg;
  int right = ((int)arg + 1) % NUMBER;
  for (;;) {
    printf("Thread %d: thinking\n", (int)arg);
    sleep(5);

    int s = pthread_mutex_lock(&mtx[left]);
    if (s != 0) {
      perror("pthread_mutex_lock");
      pthread_exit((void *)1);
    }

    if (pthread_mutex_trylock(&mtx[right]) != 0) {
      s = pthread_mutex_unlock(&mtx[left]);
      if (s != 0) {
        perror("pthread_mutex_unlock");
        pthread_exit((void *)1);
      }
      continue;
    }

    printf("Thread %d: eating\n", (int)arg);

    s = pthread_mutex_unlock(&mtx[left]);
    if (s != 0) {
      perror("pthread_mutex_unlock");
      pthread_exit((void *)1);
    }

    s = pthread_mutex_unlock(&mtx[right]);
    if (s != 0) {
      perror("pthread_mutex_unlock");
      pthread_exit((void *)1);
    }
  }

  return 0;
}

int main() {
  pthread_t t[NUMBER];
  void *res;
  int s;

  for (int i = 0; i < NUMBER; ++i) {
    s = pthread_create(&t[i], NULL, thread_func, i);
    if (s != 0) {
      perror("pthread_create");
      exit(1);
    }
  }

  for (int i = 0; i < NUMBER; ++i) {
    s = pthread_join(t[i], &res);
    if (s != 0) {
      perror("pthread_create");
      exit(1);
    }
  }
}