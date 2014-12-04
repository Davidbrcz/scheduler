#include "constants.h"
#include "debug.h"
#include "fifo.h"
#include "um_threads.h"
#include "timer_interrupt.h"

long long difftime_timespec(struct timespec t1,struct timespec t0){
  long long diff = 0;

  /* 
  diff+=(t1.tv_sec-t0.tv_sec)*1000*1000*1000;
  diff+=t1.tv_nsec-t0.tv_nsec;
  */
  /*  diff = t1.tv_nsec-t0.tv_nsec;
  if(diff<0){
    diff+=1000*1000*1000;
  }
  return diff;*/
  return (t1.tv_sec - t0.tv_sec)*1000*1000*1000 +
	  t1.tv_nsec - t0.tv_nsec;
}

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
    if(threads[i].state == IDLE && diff >= 0)
      {
	print_timespec(current_time);
	debug_printf("Thread %zu going back to READY ( missed by %lld )\n",i,diff);
	threads[i].state = READY;  
      }
  }
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
