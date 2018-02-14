#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#define NUM_OF_THREADS 4

// http://www.thegeekstuff.com/2012/04/terminate-c-thread/#comments
// int status[NUM_OF_THREADS];

void* hello(void* arg) {
  int i = *(int*) arg;
  printf("Thread %d says hello!\n", i);
  // status[i] = i;
  // http://stackoverflow.com/questions/10929503/accessing-the-return-value-using-pthread-exit
  int* status = malloc(sizeof(int));
  *status = i*10;
  pthread_exit(status);
}

int main(int argc, char *argv[]) {
  /* An array of thread identifiers, needed by pthread_join() later... */
  pthread_t tid[NUM_OF_THREADS];

  int arg[NUM_OF_THREADS];

  /* Attributes (stack size, scheduling information) for the threads. */
  pthread_attr_t attr;

  /* Get default attributes for the threads. */
  pthread_attr_init(&attr);

  /* Create one thread running each of the callbacks. */
  for (int i = 0; i < NUM_OF_THREADS; i++) {
    arg[i] = i;
    pthread_create(&tid[i], &attr, hello, &arg[i]);
  }

  int *status_ptr[NUM_OF_THREADS];

  /* Wait for all threads to terminate. */
  for (int i = 0; i < NUM_OF_THREADS; i++){
    if (pthread_join(tid[i], (void**)&(status_ptr[i])) != 0) {
      perror("pthread_join");
      exit(EXIT_FAILURE);
    }
    printf("Thread %d says goodbye with status = %d\n", i, *status_ptr[i]);
  }

}
