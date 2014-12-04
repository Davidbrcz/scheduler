#include "constants.h"
#include "debug.h"
#include "fifo.h"
#include "um_threads.h"
#include "timer_interrupt.h"

/******************************************************************************/
um_thread_id scheduler_fifo (void) {

  size_t i;
  um_thread_id elected_thread = 0;

  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);

  print_timespec(current_time);
  debug_printf ("Scheduling\n");
  long long diff = 0;
  for (i = 0; i <  um_thread_index; ++i)
  {
    diff = difftime_timespec(current_time,threads[i].awaken_date);
    bool r = timespec_lowereq_than(threads[i].awaken_date,current_time);
    //    if(threads[i].state == IDLE && diff >= 0)
    if(threads[i].state == IDLE )
      {
	//	debug_printf("Thread %zu going back to READY ( missed by %lld )\n",i,diff);
	debug_printf("Thread %zu ( missed by %lld )\n",i,diff);
	if(r){
	  print_timespec(current_time);
	  debug_printf("Thread %zu goig back ready\n",i);
	  threads[i].state = READY;  
	  threads[i].awaken_date =  null_time;
	}
      }
  }
  for(i = 1 ; i < um_thread_index; ++i)  {
    if((READY == threads[i].state) && 
	 (threads[i].priority > threads[elected_thread].priority )) {
	  elected_thread=i;
      }
  }
  um_delay(0);

  return elected_thread;
}

/******************************************************************************/
void configure_fifo_scheduler (void) {
  configure_scheduler(scheduler_fifo); /* initialize the scheduler            */
}

/******************************************************************************/
