#include <semaphore.h>	// sem_open(), sem_close(), sem_unlink(), sem_wait(), sem_post()

#ifdef __linux__
typedef sem_t semaphore_t;
#endif

#ifdef __APPLE__

typedef struct {
  char *name;
  sem_t *sem;
} semaphore_t;

#endif
