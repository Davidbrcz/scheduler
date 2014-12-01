#include<stdbool.h>

#include "debug.h"
#include "constants.h"
#include "table_driven.h"
#include "um_threads.h"
#include "timer_interrupt.h"

schedule_table *the_table;
um_thread_id current_thread;

/******************************************************************************/
um_thread_id scheduler_table_driven (void) {
  TODO(Exercise 3)
  return current_thread;
}

/******************************************************************************/
void configure_table_driven_scheduler (schedule_table *table) {
  configure_scheduler(scheduler_table_driven);   /* initialize the scheduler */
  TODO(Exercise 3)
}

/******************************************************************************/
