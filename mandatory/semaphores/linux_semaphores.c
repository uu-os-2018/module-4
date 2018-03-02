#include <stdio.h> // perror()
#include <stdlib.h> // malloc()

#include "semaphores.h"

semaphore_t *s_init(unsigned int value) {
  semaphore_t *sem = malloc(sizeof(sem_t));

 if (sem_init(sem, 0, value) == -1) {
   perror("Initializing new semaphore");
   abort();
 }
 return sem;
}

void s_wait(semaphore_t *sem) {
  if (sem_wait(sem) == -1) {
    perror("Wating on sempahore failed");
    abort();
  }
}

void s_signal(semaphore_t *sem) {
  if (sem_post(sem) == -1) {
    perror("Signaling on semaphore failed");
    abort();
  }
}

void s_destroy(semaphore_t *sem) {
  if (sem_destroy(sem) == -1) {
    perror("Destroying semaphore failed");
    abort();
  }
}
