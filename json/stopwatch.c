#include "stopwatch.h"

#ifdef __APPLE__

void stopwatch_init(stopwatch_t *stopwatch) {
  mach_timebase_info(&stopwatch->timebase);
  stopwatch->start = 0;
  stopwatch->end = 0;
}

void stopwatch_start(stopwatch_t *stopwatch) { stopwatch->start = mach_absolute_time(); }

uint64_t stopwatch_end(stopwatch_t *stopwatch) {
  stopwatch->end = mach_absolute_time();
  return (stopwatch->end - stopwatch->start) * stopwatch->timebase.numer /
         stopwatch->timebase.denom;
}

#else

void stopwatch_init(stopwatch_t *stopwatch) {
  (void)stopwatch;
  // noop
}

void stopwatch_start(stopwatch_t *stopwatch) {
  clock_gettime(CLOCK_MONOTONIC, &stopwatch->start_ts);
}

uint64_t stopwatch_end(stopwatch_t *stopwatch) {
  struct timespec end_ts;
  clock_gettime(CLOCK_MONOTONIC, &end_ts);
  uint64_t start_ns = (uint64_t)stopwatch->start_ts.tv_sec * 1000000000ULL + stopwatch->start_ts.tv_nsec;
  uint64_t end_ns = (uint64_t)end_ts.tv_sec * 1000000000ULL + end_ts.tv_nsec;
  return end_ns - start_ns;
}

#endif // __APPLE__
