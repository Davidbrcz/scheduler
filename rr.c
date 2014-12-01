#include "constants.h"
#include "rr.h"
#include "um_threads.h"
#include "timer_interrupt.h"
#include "debug.h"

/******************************************************************************/
um_thread_id scheduler_rr (void) {
  TODO(Exercise 1)
  return 0;
}

/******************************************************************************/
void configure_rr_scheduler (uint32_t period) {
  configure_scheduler(scheduler_rr);  /* initialize the scheduler            */
  TODO(Exercise 1)
}

/******************************************************************************/
