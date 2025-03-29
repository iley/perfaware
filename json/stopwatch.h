#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include <stdint.h>

#ifdef __APPLE__
#include <mach/mach_time.h>

typedef struct {
  mach_timebase_info_data_t timebase;
  uint64_t start;
  uint64_t end;
} stopwatch_t;

#else
#include <time.h>

typedef struct {
  struct timespec start_ts;
} stopwatch_t;

#endif // __APPLE__

void stopwatch_init(stopwatch_t *stopwatch);
void stopwatch_start(stopwatch_t *stopwatch);
// Retruns the delta in nanoseconds.
uint64_t stopwatch_end(stopwatch_t *stopwatch);

#endif // STOPWATCH_H_
