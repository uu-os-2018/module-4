/**
 * Routines for high precision timing measurements.
 *
 * Author: Andreas Sandberg <andreas.sandberg@it.uu.se>
 *
 * $Id: timing.c 1009 2011-07-28 15:02:57Z ansan501 $
 */

#include "timing.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Use the monotonic high resolution clock by default. This clock
 * can't be set and is guaranteed not to jump backwards. The clock has
 * no well defined starting point and is only suitable for measuring
 * time intervals. */
#if defined(CLOCK_MONOTONIC)
#define CLOCK_ID CLOCK_MONOTONIC
#else
#error No suitable clock type found.
#endif

static void
checked_gettime(struct timespec *ts)
{
        if (clock_gettime(CLOCK_ID, ts) != 0) {
                perror("clock_gettime failed");
                abort();
        }
}

double
timing_precision()
{
        struct timespec ts;

        if (clock_getres(CLOCK_ID, &ts) != 0) {
                perror("clock_getres failed");
                abort();
        }

        return (ts.tv_sec + ts.tv_nsec * 1E-9);
}

void
timing_start(struct timespec *ts_start)
{
        checked_gettime(ts_start);
}

double
timing_stop(struct timespec *ts_start)
{
        struct timespec ts;

        checked_gettime(&ts);

        /* Sanity check, make sure that the stop time is after the start
         * time. */
        assert(ts.tv_sec > ts_start->tv_sec ||
               (ts.tv_sec == ts_start->tv_sec &&
                ts.tv_nsec >= ts_start->tv_nsec));

        return ts.tv_sec - ts_start->tv_sec +
                (ts.tv_nsec - ts_start->tv_nsec) * 1E-9;
}

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 8
 * indent-tabs-mode: nil
 * c-file-style: "linux"
 * End:
 */
