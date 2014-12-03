#include<stdbool.h>

#include "debug.h"
#include "constants.h"
#include "table_driven.h"
#include "um_threads.h"
#include "timer_interrupt.h"

schedule_table *the_table;
um_thread_id current_thread;
int i = 0;
/******************************************************************************/
um_thread_id scheduler_table_driven (void) {
  
  i = (i+1) % the_table->size;

  current_thread=the_table->table_entries[i].a_thread;
  setup_timer(the_table->table_entries[i].slot_time,false);

  return current_thread;
}

/******************************************************************************/
void configure_table_driven_scheduler (schedule_table *table) {
  configure_scheduler(scheduler_table_driven);   /* initialize the scheduler */
  
  the_table=table;

  current_thread=the_table->table_entries[i].a_thread;
  setup_timer(the_table->table_entries[i].slot_time,false);
}

/******************************************************************************/
