#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

#include <mach/mach_time.h>

typedef struct {
  mach_timebase_info_data_t timebase;
  uint64_t start;
  uint64_t end;
} timer_t;

void timer_init(timer_t *timer);
void timer_start(timer_t *timer);
// Retruns the delta in nanoseconds.
uint64_t timer_end(timer_t *timer);

#endif // TIMER_H_
