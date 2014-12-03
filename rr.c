#include "constants.h"
#include "rr.h"
#include "um_threads.h"
#include "timer_interrupt.h"
#include "debug.h"

/******************************************************************************/
/*
  This function will be called each time a quantum is consumed 
  @return : the index of the thread to be executed next
*/
um_thread_id scheduler_rr (void) {

  return (get_current_context_id() +1) % um_thread_index;

}

/******************************************************************************/
void configure_rr_scheduler (uint32_t period) {
  configure_scheduler(&scheduler_rr);  /* initialize the scheduler            */
  if(period != 0) {
    setup_timer(period,true);
  }
}

/******************************************************************************/
