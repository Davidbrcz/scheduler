#include "constants.h"
#include "debug.h"
#include "fifo.h"
#include "um_threads.h"
#include "timer_interrupt.h"

/******************************************************************************/
um_thread_id scheduler_fifo (void) {
  int i;
  um_thread_id elected_thread = 0;

  TODO(Exercise 1)

  return elected_thread;
}

/******************************************************************************/
void configure_fifo_scheduler (void) {
  configure_scheduler(scheduler_fifo); /* initialize the scheduler            */
}

/******************************************************************************/
