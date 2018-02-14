/**
 * Routines for high precision timing measurements.
 *
 * Author: Andreas Sandberg <andreas.sandberg@it.uu.se>
 *
 * $Id: timing.h 1009 2011-07-28 15:02:57Z ansan501 $
 */

#ifndef TIMING_H
#define TIMING_H

#include <time.h>

/**
 * Get the precision of the timer exposed by the underlying OS.
 *
 * \return Precision in seconds
 */
extern double timing_precision();

/**
 * Start the timing measurements.
 *
 * \param t Pointer to a timespec struct to store the start time in.
 */
extern void timing_start(struct timespec *ts);

/**
 * Stops the timer. The timer must have been started using
 * timing_start() before timing_stop() is called.
 *
 * \param ts_start Pointer to a timespec struct with the start time.
 * \return Length in seconds of the timed interval.
 */
extern double timing_stop(struct timespec *ts_start);

#endif

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 8
 * indent-tabs-mode: nil
 * c-file-style: "linux"
 * End:
 */
