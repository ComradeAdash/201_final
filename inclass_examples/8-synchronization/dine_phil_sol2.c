#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 10

static char buf[SIZE] = {0};
static int in = 0, out = 0;
static sem_t filled_cnt;
static sem_t avail_cnt;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void *thread_func(void *arg) {
  int s;
  for (;;) {
    sleep(1);
    if (sem_wait(&filled_cnt) == -1) {
      perror("sem_wait");
      pthread_exit((void *)1);
    }
    s = pthread_mutex_lock(&mtx);
    if (s != 0) {
      perror("pthread_mutex_lock");
      exit(1);
    }

    // consume
    printf("Consumed: %d\n", buf[out]);
    out = (out + 1) % SIZE;

    s = pthread_mutex_unlock(&mtx);
    if (s != 0) {
      perror("pthread_mutex_lock");
      exit(1);
    }

    if (sem_post(&avail_cnt) == -1) {
      perror("sem_post");
      pthread_exit((void *)1);
    }
  }

  return 0;
}

int main() {
  pthread_t t1;
  void *res;
  int s;
  if (sem_init(&filled_cnt, 0, 0) == -1) {
    perror("sem_init");
    exit(1);
  }
  if (sem_init(&avail_cnt, 0, SIZE) == -1) {
    perror("sem_init");
    exit(1);
  }

  s = pthread_create(&t1, NULL, thread_func, NULL);
  if (s != 0) {
    perror("pthread_create");
    exit(1);
  }

  for (int i = 0;; i++) {
    if (sem_wait(&avail_cnt) == -1) {
      perror("sem_wait");
      exit(1);
    }
    s = pthread_mutex_lock(&mtx);
    if (s != 0) {
      perror("pthread_mutex_lock");
      exit(1);
    }

    // produce
    buf[in] = i;
    printf("Produced: %d in %d\n", buf[in], in);
    in = (in + 1) % SIZE;

    s = pthread_mutex_unlock(&mtx);
    if (s != 0) {
      perror("pthread_mutex_lock");
      exit(1);
    }

    if (sem_post(&filled_cnt) == -1) {
      perror("sem_post");
      exit(1);
    }
  }

  s = pthread_join(t1, &res);
  if (s != 0) {
    perror("pthread_create");
    exit(1);
  }
}