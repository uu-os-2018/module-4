#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#define NUM_OF_THREADS 4

void* hello(void* arg) {
  int i = *(int*) arg;
  printf("  thread %d - hello\n", i);
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  /* An array of thread identifiers, needed by
     pthread_join() later. */
  pthread_t tid[NUM_OF_THREADS];

  /* An array to hold argument data to the hello()
     start routine for each thread. */
  int arg[NUM_OF_THREADS];

  /* Attributes (stack size, scheduling information
     etc) for the new threads. */
  pthread_attr_t attr;

  /* Get default attributes for the threads. */
  pthread_attr_init(&attr);

  printf("main() - before creaing new threads\n");

  /* Create new threads, each executing the
     hello() function. */
  for (int i = 0; i < NUM_OF_THREADS; i++) {
    arg[i] = i;
    pthread_create(&tid[i], &attr, hello, &arg[i]);
  }

  /* Wait for all threads to terminate. */
  for (int i = 0; i < NUM_OF_THREADS; i++){
    if (pthread_join(tid[i], NULL) != 0) {
      perror("pthread_join");
      exit(EXIT_FAILURE);
    }
    printf("main() - thread %d terminated\n", i);
  }

  printf("main() - all new threads terminated\n");

  exit(EXIT_SUCCESS);
}
