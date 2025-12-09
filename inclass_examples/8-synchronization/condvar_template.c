#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int main() {
  int s = pthread_mutex_lock(&mtx);

  if (s != 0) {
    perror("pthread_mutex_lock");
    exit(1);
  }

  while (/* Check if the shared variable is not in the state we want, e.g., if there is nothing to
            consume  */ )
    /* Use while, not if, for safety. Although pthread_cond_signal should've
     * been called because the shared variable were in the state we want, other
     * threads might have woken up first and changed the shared variable. */
    pthread_cond_wait(&cond, &mtx);

  // Do the necessary work with the shared variable, e.g., consume.

  s = pthread_mutex_unlock(&mtx);
  if (s != 0) {
    perror("pthread_mutex_lock");
    exit(1);
  }
}