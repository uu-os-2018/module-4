#include <unistd.h> // mkstemp()
#include <string.h> // strcpy()
#include <stdio.h>	// perror()
#include <stdlib.h>	// malloc()



#include "semaphores.h"

/*
  Unnamed POSIX semaphores are not implemented on macOS (aka OS X).
  Use named semaphores from sempahore.h to implement a generic API to
  un-named semaphores.
*/

void cleanup(semaphore_t *sem) {
  if (sem_close(sem->sem) == -1) {
    perror("sem_close()");
    abort();
  }

  if (sem_unlink(sem->name) == -1) {
    perror("sem_unlink()");
    abort();
  }

  free(sem->name);
  free(sem);
}

void perror_and_abort(semaphore_t *sem, const char* msg) {
  perror(msg);
  cleanup(sem);
  abort();
}

semaphore_t *s_init(unsigned int value) {
  semaphore_t *sem = malloc(sizeof(semaphore_t));

  sem->name = strdup("/tmp/semaphore.XXXXXX");
  mktemp(sem->name);

  sem->sem = sem_open(sem->name, O_CREAT | O_EXCL, 0, value);

  if (sem->sem == SEM_FAILED) {
    perror_and_abort(sem, "sem_open()");
  }
  return sem;
}

void s_wait(semaphore_t *sem) {
  if (sem_wait(sem->sem) == -1) {
    perror_and_abort(sem, "sem_wait()");
  }
}

void s_signal(semaphore_t *sem) {
  if (sem_post(sem->sem) == -1) {
    perror_and_abort(sem, "sem_post()");
  }
}

void s_destroy(semaphore_t *sem) {
  cleanup(sem);
}
