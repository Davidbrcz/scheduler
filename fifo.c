#include "constants.h"
#include "debug.h"
#include "fifo.h"
#include "um_threads.h"
#include "timer_interrupt.h"

long long difftime_timespec(struct timespec t1,struct timespec t0){
  long  long diff = 0;

  diff+=(t1.tv_sec-t0.tv_sec)*1000*1000*1000;
  diff+=t1.tv_nsec-t0.tv_nsec;

  return diff;
}

/******************************************************************************/
um_thread_id scheduler_fifo (void) {
  size_t i;
  um_thread_id elected_thread = 0;

  struct timespec time;
  clock_gettime(CLOCK_REALTIME, &time);

  printf ("----------\n");
  for (i = 0; i <  um_thread_index; ++i)
  {
    if(threads[i].state == IDLE && difftime_timespec(threads[i].awaken_date,time)<2)
      {
	print_timespec(time);
	debug_printf("Thread %zu going back to READY\n",i);
	threads[i].state = READY;  
      }
  }
  printf ("----------\n");
  for(i = 1 ; i < um_thread_index; ++i)  {
    if((READY == threads[i].state) && 
	 (threads[i].priority > threads[elected_thread].priority )) {
	  elected_thread=i;
      }
  }

  return elected_thread;
}

/******************************************************************************/
void configure_fifo_scheduler (void) {
  configure_scheduler(scheduler_fifo); /* initialize the scheduler            */
}

/******************************************************************************/
