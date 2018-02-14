#include <pthread.h> // pthread_attr_init(), pthread_create(), pthread_join ()
#include <stdio.h>   // printf()
#include <unistd.h>  // sleep()
#include <stdlib.h>  // exit(), EXIT_FAILURE

#define NUM_OF_THREADS 2

void* worker(void *arg) {
  int id = *(int*) arg;
  printf("PID = <%ld> Thread %d says: Hello!\n", (long) getpid(), id);
  for (int i = 2; i > 0; i--) {
    printf("PID = <%ld> Thread %d says: I've got %d seconds to live.\n", 
	   (long) getpid(), id, i);
    sleep(1);
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  /* An array of thread identifiers, needed by pthread_join() later... */      
  pthread_t tid[NUM_OF_THREADS];
  /* An array with argument data for each thread. */
  int      targ[NUM_OF_THREADS]; 
  /* Attributes (stack size, scheduling information) for the threads. */
  pthread_attr_t attr;
  /* Get default attributes for the threads. */
  pthread_attr_init(&attr);
  printf("PID = <%ld> ==> Lets create some threads.\n", (long) getpid());

  /* Create threads. */
  for (int i = 0; i < NUM_OF_THREADS; i++) {
    targ[i] = i;
    pthread_create(&tid[i], &attr, worker, &targ[i]); 	
  } 

  /* Create a new process - will the threads be duplicated or not? */
  switch(fork()) {
  case -1:
    perror("Forked failed.");
    exit(EXIT_FAILURE);
  case 0:
   printf("PID = <%ld> ==> Child after fork()!\n", (long) getpid());
   break;
  default:
    printf("PID = <%ld> ==> Parent after fork()!\n", (long) getpid());
  }

  /* Wait for all threads to terminate. */
  for (int i = 0; i < NUM_OF_THREADS; i++){ 
    if (pthread_join(tid[i], NULL) == 0)
      printf("PID = <%ld> ==> Joining thread %d.\n", (long) getpid(), i);
    else {
      printf("PID = <%ld> ==> No thread to join.\n", (long) getpid());
    }
  }
}
