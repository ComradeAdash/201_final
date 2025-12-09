#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static pthread_mutex_t mutex0 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

static void *thread0(void *arg) {
  pthread_mutex_lock(&mutex0);
  printf("thread0: mutex0\n");
  pthread_mutex_lock(&mutex1);
  printf("thread0: mutex1\n");
  pthread_mutex_unlock(&mutex0);
  pthread_mutex_unlock(&mutex1);
  pthread_exit(0);
}

static void *thread1(void *arg) {
  pthread_mutex_lock(&mutex1);
  printf("thread1: mutex1\n");
  pthread_mutex_lock(&mutex0);
  printf("thread1: mutex0\n");
  pthread_mutex_unlock(&mutex1);
  pthread_mutex_unlock(&mutex0);
  pthread_exit(0);
}

int main() {
  pthread_t t0;
  pthread_t t1;

  if (pthread_create(&t0, NULL, thread0, NULL) != 0)
    perror("pthread_create");

  if (pthread_create(&t1, NULL, thread1, NULL) != 0)
    perror("pthread_create");

  if (pthread_join(t0, NULL) != 0)
    perror("pthread_join");
  if (pthread_join(t1, NULL) != 0)
    perror("pthread_join");

  exit(EXIT_SUCCESS);
}