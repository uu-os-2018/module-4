/**
 * Critical sections and mutual exclusion
 *
 * Multiple threads incrementing and decrementing the same shared variable.
 *
 * Author: Nikos Nikoleris <nikos.nikoleris@it.uu.se>
 *
 */

#include <stdio.h>     /* printf(), fprintf() */
#include <stdlib.h>    /* abort() */
#include <pthread.h>   /* pthread_... */
#include <stdbool.h>   /* true, false */

#include "timing.h"


/* Number of threads that will increment the shared variable */
#define INC_THREADS 5
/* Value by which the threads increment the shared variable */
#define INCREMENT 2
/* Iterations performed incrementing the shared variable */
#define INC_ITERATIONS 2000000

/* Number of threads that will try to decrement the shared variable */
#define DEC_THREADS 4
/* Value by which the threads increment the shared variable */
#define DECREMENT 2
/* Iterations performed decrementing the shared variable.*/
#define DEC_ITERATIONS (INC_ITERATIONS * INC_THREADS * INCREMENT / DEC_THREADS / DECREMENT)

/* Shared variable */
volatile int counter;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* Shared variable used to implement a spinlock */
volatile int lock = false;

/* Access to the shared counter should be protected by a mutex */
void *
inc_mutex(void *arg __attribute__((unused)))
{
    int i;

    /* TODO 1: Protect access to the shared variable with a mutex lock */

    for (i = 0; i < INC_ITERATIONS; i++) {
        counter += INCREMENT;
    }

    return NULL;
}

void *
dec_mutex(void *arg __attribute__((unused)))
{
    int i;

    /* TODO 1: Protect access to the shared variable with a mutex lock */

    for (i = 0; i < DEC_ITERATIONS; i++) {
        counter -= DECREMENT;
    }

    return NULL;
}

void spin_lock() {
    /* TODO 2: Implement a test-and-set spinlock */
}

void spin_unlock() {
    /* TODO 2: Implement a test-and-set spinlock */
}

/* Access to the shared counter should be protected by a test-and-set spinlock */
void *
inc_tas_spinlock(void *arg __attribute__((unused)))
{
    int i;

    /* TODO 2: Add the spin_lock() and spin_unlock() operations */

    for (i = 0; i < INC_ITERATIONS; i++) {
        counter += INCREMENT; 
    }

    return NULL;
}

void *
dec_tas_spinlock(void *arg __attribute__((unused)))
{
    int i;

    /* TODO 2: Add the spin_lock() and spin_unlock() operations */

    for (i = 0; i < DEC_ITERATIONS; i++) {
        counter -= DECREMENT; 
    }

    return NULL;
}


/* Access to the shared counter should be implemented using the atomic
 * instructions */
void *
inc_atomic(void *arg __attribute__((unused)))
{
    int i;

    /* TODO 3: Use atomic primitives to manipulate the shared variable */

    for (i = 0; i < INC_ITERATIONS; i++) {
        counter += DECREMENT; // You need to replace this
    }

    return NULL;
}

void *
dec_atomic(void *arg __attribute__((unused)))
{
    int i;

    /* TODO 3: Use atomic primitives to manipulate the shared variable */

    for (i = 0; i < DEC_ITERATIONS; i++) {
        counter += DECREMENT; // You need to replace this
    }

    return NULL;
}


/***********************************************************/
/* NOTE: You don't need to modify anything below this line */
/***********************************************************/

struct func_test_t {
    void * (*inc)(void *);
    void * (*dec)(void *);
    char *name;
};

struct func_test_t func_test[] = {
    { .inc = inc_mutex,        .dec = dec_mutex,        .name = "Test mutex" },
    { .inc = inc_tas_spinlock, .dec = dec_tas_spinlock, .name = "Test tas spinlock" },
    { .inc = inc_atomic,       .dec = dec_atomic,       .name = "Test atomic" },
    { .inc = NULL,             .dec = NULL,             .name = "stop" }
};


typedef struct {
    pthread_t thread;
    int id;
    void *(*func)(void *);
    void *arg;
    double run_time;
} thread_conf_t;


void *
thread_func(void *_conf)
{
    struct timespec ts;
    thread_conf_t *conf = (thread_conf_t *)_conf;

    timing_start(&ts);

    conf->func(conf->arg);

    conf->run_time = timing_stop(&ts);

    pthread_exit(0);
}


void
print_stats(thread_conf_t *threads, int nthreads, int niterations)
{
    double run_time_sum = 0;

    printf("Statistics:\n");
    for (int i = 0; i < nthreads; i++) {
	thread_conf_t *t = &threads[i];
	printf("\tThread %i: %.4f sec (%.4e iterations/s)\n",
	       i, t->run_time,
	       niterations / nthreads / t->run_time);
	run_time_sum += t->run_time;
    }

    printf("\tAverage execution time: %.4f s\n"
	   "\tAvergage iterations/second: %.4e\n",
	   run_time_sum / nthreads,
	   niterations / nthreads / run_time_sum);
}

int
main()
{
    struct func_test_t *_func_test = func_test;

    while (_func_test->inc && _func_test->dec) {
        int n, nthreads = 0;
        thread_conf_t thread_conf[INC_THREADS + DEC_THREADS];

        pthread_setconcurrency(INC_THREADS + DEC_THREADS);

        counter = 0;

        pthread_setconcurrency(INC_THREADS + DEC_THREADS + 1);
        /* Create the threads */
        for (n = 0; n < INC_THREADS; n++) {
            thread_conf_t *conf = &thread_conf[nthreads];
            conf->id = nthreads;
            conf->func = _func_test->inc;
            if (pthread_create(&conf->thread, NULL, thread_func, conf) != 0) {
                perror("pthread_create");
                abort();
            }
            nthreads++;
        }
        for (n = 0; n < DEC_THREADS; n++) {
            thread_conf_t *conf = &thread_conf[nthreads];
            conf->id = nthreads;
            conf->func = _func_test->dec;
            if (pthread_create(&conf->thread, NULL, thread_func, conf) != 0) {
                perror("pthread_create");
                abort();
            }
            nthreads++;
        }

        /* Wait for them to complete */
        for (n = 0; n < nthreads; n++)
            if (pthread_join(thread_conf[n].thread, NULL) != 0) {
                perror("pthread_join");
                abort();
            }

        printf("===> %s\n", _func_test->name);
        if (counter != 0) {
            printf("     counter expected value:%10d\n", 0);
            printf("     counter actual value:  %10d\n", counter);

            printf("     Failure\n");
        } else {
            printf("     Success\n");
            print_stats(thread_conf, nthreads, INC_ITERATIONS + DEC_ITERATIONS);
        }
        _func_test++;
    }
    return 0;
}

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * c-file-style: "stroustrup"
 * End:
 */
