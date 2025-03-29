#include "timer.h"

void timer_init(timer_t *timer) {
  mach_timebase_info(&timer->timebase);
  timer->start = 0;
  timer->end = 0;
}

void timer_start(timer_t *timer) { timer->start = mach_absolute_time(); }

uint64_t timer_end(timer_t *timer) {
  timer->end = mach_absolute_time();
  return (timer->end - timer->start) * timer->timebase.numer /
         timer->timebase.denom;
}
