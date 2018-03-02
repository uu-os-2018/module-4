#include <semaphore.h>	// sem_open(), sem_close(), sem_unlink(), sem_wait(), sem_post()

#ifdef __linux__
typedef sem_t semaphore_t;
#endif

#ifdef __APPLE__

#include <stdio.h>	// L_tmpnam
#include <stdlib.h>	// malloc()

#define MAX_NAME_LEN L_tmpnam

typedef struct {
  char *name;
  sem_t *sem;
} semaphore_t;

#endif
