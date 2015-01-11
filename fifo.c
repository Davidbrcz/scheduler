#include "constants.h"
#include "debug.h"
#include "fifo.h"
#include "um_threads.h"
#include "timer_interrupt.h"

/******************************************************************************/
/*
  Will look into the list of threads, put back the IDLE ones which should awake
  and if there is at least one IDLE thread left, then call timer_lowest_awken_date
  to compute the date and set the timer
*/
void update_thread_status(){

  size_t i;

  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);

  print_timespec(current_time);
  debug_printf ("Scheduling\n");

  bool need_to_setup_timer = false;
  for (i = 0; i <  um_thread_index; ++i)
  {
    if(threads[i].state == IDLE)
      {
	//If current time is after the awaken time, the the thread is going back up
	long r =  difftime_timespec(threads[i].awaken_date,current_time);
	if(r<1000) {
	  print_timespec(current_time);
	  debug_printf("Thread %zu goig back ready\n",i);
	  threads[i].state = READY;  
	  threads[i].awaken_date =  infinite_time;
	} else {
	  need_to_setup_timer = true;
	}
      }
  }

  if(need_to_setup_timer){
    timer_lowest_awken_date();
  }
}
um_thread_id scheduler_fifo (void) {

  size_t i;

  um_thread_id elected_thread = get_current_context_id();  
  update_thread_status();
  
  for(i = 0 ; i < um_thread_index; ++i)  {
    if(READY == threads[i].state && (threads[elected_thread].state == IDLE ||  
				      threads[i].priority > threads[elected_thread].priority)) {
	  elected_thread=i;
      }
  }

  return elected_thread;
}

/******************************************************************************/
void configure_fifo_scheduler (void) {
  configure_scheduler(&scheduler_fifo); /* initialize the scheduler            */
}

/******************************************************************************/
