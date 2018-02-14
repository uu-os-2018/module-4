#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> // strtol()
#include <unistd.h>  // sleep(), usleep()

#define DELAY_MAX 500

#define DEFAULT_NUMBER_OF_ITERATIONS 1000
#define DEFAULT_RANDOM_USLEEP_MIN 20
#define DEFAULT_RANDOM_USLEEP_MAX 200
#define DEFAULT_BALANCE 0

unsigned int NUMBER_OF_ITERATIONS;
unsigned int RANDOM_USLEEP_MIN;
unsigned int RANDOM_USLEEP_MAX;

/* TODO: adding or removing the volatile keyword ... run the program ...
   observere how the likelyhood of data races are (heavily) affected ... */


/* http://brooker.co.za/blog/2013/01/06/volatile.html */

int BALANCE;
volatile int VOLATILE_BALANCE;

/* Makes the caller take a sleep for a random number of microseconds in the
   interval [min, max]. The sleep will be repeated weight number of times. */
inline void random_usleep(unsigned int *seed, unsigned int weight) {
  /* The actual time slept when calling usleep() may be longer, due to system
     latencies and possible limitations in the timer resolution of the
     hardware.

     This means that usleep(0) will most likely make the caller sleep for a
     small amount of time. */

  if (RANDOM_USLEEP_MIN == 0 && RANDOM_USLEEP_MAX == 0) {
    return;
  }

  int n = RANDOM_USLEEP_MIN + (rand_r(seed) % (RANDOM_USLEEP_MAX - RANDOM_USLEEP_MIN + 1));
   for (int i = 0; i < weight; i++) {
    usleep(n);
  }
}

void inc(unsigned int* seed, int *n) {
  int old = *n;
  random_usleep(seed, 1);
  int new = old + 1;
  *n = new;
}

void dec(unsigned int* seed, int *n) {
  int old = *n;
  random_usleep(seed, 1);
  int new = old - 1;
  *n = new;
}

void* increment(void *seed) {
  for (int i = 0; i < NUMBER_OF_ITERATIONS; i++) {
    BALANCE++;
    // inc((unsigned int*) seed, &BALANCE);
    // random_usleep((unsigned int*) seed, 1);
  }
  return NULL;
}

void* decrement(void *seed) {
  for (int i = 0; i < NUMBER_OF_ITERATIONS; i++) {
    BALANCE--;
    // dec((unsigned int*) seed, &BALANCE);
    // random_usleep((unsigned int*) seed, 1);
  }

  return NULL;
}

void* vincrement(void *seed) {
  for (int i = 0; i < NUMBER_OF_ITERATIONS; i++) {
     VOLATILE_BALANCE++;
  }
  return NULL;
}

void* vdecrement(void *seed) {
  for (int i = 0; i < NUMBER_OF_ITERATIONS; i++) {
    VOLATILE_BALANCE--;
  }

  return NULL;
}


int parse_int(int argc, char *argv[], int index, int default_value, volatile int *target) {
  char *endptr;
  long result;
  long parse = 0;

  if (argc > index) {
    parse = strtol(argv[index], &endptr, 10);
    if (endptr == argv[index] || parse < 0){
      /* invalid parse */
      result = 1;

      *target = default_value;
    } else {
      result = 0;
      *target = (int) parse;
    }

  } else {
    result = 1;
    *target = default_value;
  }

  return result;
}

int parse_unsigned_int(int argc, char *argv[], int index, unsigned int default_value, unsigned int *target) {
  char *endptr;
  long result;
  long parse = 0;

  if (argc > index) {
    parse = strtol(argv[index], &endptr, 10);
    if (endptr == argv[index] || parse < 0){
      /* invalid parse */
      result = 1;

      *target = default_value;
    } else {
      result = 0;
      *target = parse;
    }
    
  } else {
    result = 1;
    *target = default_value;
  }
  return result;
}

void initialize(int argc, char *argv[]) {
  char suffix_str[2][16] = {"", " (default)"};

  int r[5];

  r[0] = parse_unsigned_int(argc, argv, 1, DEFAULT_NUMBER_OF_ITERATIONS, &NUMBER_OF_ITERATIONS);
  r[1] = parse_unsigned_int(argc, argv, 2, DEFAULT_RANDOM_USLEEP_MIN, &RANDOM_USLEEP_MIN);
  r[2] = parse_unsigned_int(argc, argv, 3, DEFAULT_RANDOM_USLEEP_MAX, &RANDOM_USLEEP_MAX);
  r[3] = parse_int(argc, argv, 4, DEFAULT_BALANCE, &BALANCE);
  r[4] = parse_int(argc, argv, 4, DEFAULT_BALANCE, &VOLATILE_BALANCE);


  printf("\n");
  printf("     Number of iterations: %d%s\n", NUMBER_OF_ITERATIONS, suffix_str[r[0]]);
  printf("Min sleep (micro seconds): %d%s\n", RANDOM_USLEEP_MIN, suffix_str[r[1]]);
  printf("Max sleep (micro seconds): %d%s\n", RANDOM_USLEEP_MAX, suffix_str[r[2]]);
  printf("          Initial balance: %d%s\n", BALANCE, suffix_str[r[3]]);
  printf(" Initial volatile balance: %d%s\n", BALANCE, suffix_str[r[3]]);
}

int main(int argc, char *argv[]) {
  /* Number of iterations */
  initialize(argc, argv);

  srand(time(NULL));
  unsigned int seed[4];
  seed[0] = rand();
  seed[1] = rand();
  seed[2] = rand();
  seed[3] = rand();

  /* An array of thread identifiers, needed by pthread_join() later... */
  pthread_t tid[4];

  /* Attributes (stack size, scheduling information) for the threads.  */
  pthread_attr_t attr;

  /* Get default attributes for the threads. */
  pthread_attr_init(&attr);

  /* Create one increment thread and one decrement thread. */
  pthread_create(&tid[0], &attr, increment, &seed[0]);
  pthread_create(&tid[1], &attr, decrement, &seed[1]);

  pthread_create(&tid[0], &attr, vincrement, &seed[0]);
  pthread_create(&tid[0], &attr, vdecrement, &seed[0]);

  /* Wait for all threads to terminate. */
  for (int i = 0; i < 2; i++){
    pthread_join(tid[i], NULL);
  }

  /* Print results. */
  printf("\n");
  printf("            Final balance: %d\n", BALANCE);
  printf("   Final volatile balance: %d\n\n", VOLATILE_BALANCE);
}
