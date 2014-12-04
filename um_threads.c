#include<signal.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/time.h>

#include "constants.h"
#include "debug.h"
#include "um_threads.h"
#include "timer_interrupt.h"
#include<time.h>
/******************************************************************************/
um_thread_t threads[MAX_THREADS];

uint32_t um_thread_index = 0;   

int sched_current_context_id = 0;  /* Current thread executing                */
ucontext_t *sched_context;         /* a pointer to the running thread         */

scheduler_function the_scheduler;

/******************************************************************************/
um_thread_id um_thread_create
(void (*function)(void), 
 stack_size_t stack_size,
 priority_t priority)
{
  void *stack;
  int err;

  err = getcontext(&(threads[um_thread_index].um_context));
  if (err < 0)
    perror("getcontext");

  if (stack_size == 0)
    threads[um_thread_index].stack_size = STACKSIZE;
  else
    threads[um_thread_index].stack_size = stack_size;

  /* Allocate memory for the thread stack */
  stack = malloc(threads[um_thread_index].stack_size);
  if (stack == NULL) {
    perror("malloc");
    exit(1);
  }

  /* Initialze the  ucontext structure:
   * - stack and stack size
   * - reset sigmask
   */
  threads[um_thread_index].um_context.uc_stack.ss_sp    = stack;
  threads[um_thread_index].um_context.uc_stack.ss_size  
    = threads[um_thread_index].stack_size;

  threads[um_thread_index].um_context.uc_stack.ss_flags = 0;
  sigemptyset(&threads[um_thread_index].um_context.uc_sigmask);
  
  /* Attach user function */
  makecontext(&threads[um_thread_index].um_context, function, 0);
  
  /* Update internal arrays of threads */
  threads[um_thread_index].tid = um_thread_index;
  threads[um_thread_index].priority = priority;
  threads[um_thread_index].state = READY;
  
  debug_printf("Created thread context: %p, tid %d, function %p\n", 
	 &(threads[um_thread_index].um_context), threads[um_thread_index].tid, 
	 function);

  return um_thread_index++;
}

/******************************************************************************/
ucontext_t *get_context (um_thread_id tid) {
  return &(threads[tid].um_context);
}

/******************************************************************************/
ucontext_t *get_current_context (void) {
  return sched_context;
}

/******************************************************************************/
um_thread_id get_current_context_id (void) {
  return sched_current_context_id;
}

/******************************************************************************/
void start_scheduler (void) {
  sched_current_context_id = 0;
  sched_context = get_context(sched_current_context_id);
  debug_printf("Starting scheduler @ %p\n", sched_context);
  scheduler();
}

/******************************************************************************/
/* The scheduling algorithm; selects the next context to run, then starts it. */

void scheduler(void)
{
  um_thread_id previous = sched_current_context_id;

  /* 
     if the thread is not waiting something, the we move 
     it back from RUNNING to READY
  */
  if(threads[sched_current_context_id].state != IDLE) {
    threads[sched_current_context_id].state = READY;  
  }
  sched_current_context_id = the_scheduler ();

  sched_context = get_context (sched_current_context_id);
  threads[sched_current_context_id].state = RUNNING;  

  print_timestamp();
  debug_printf("Swithching from %d to %d\n", 
	       previous, sched_current_context_id);
  
  setcontext(sched_context); /* go */
}

/******************************************************************************/
void um_thread_yield (void) {
  scheduler();
}

/******************************************************************************/
void configure_scheduler (scheduler_function s) {
  the_scheduler = s;
}
/******************************************************************************/
#define milliard 1000000000

/*d_ms must be < 999 */

void um_delay(uint32_t d_ms){
  if(d_ms>0){
    um_thread_id id = get_current_context_id();
    struct timespec time;

    clock_gettime(CLOCK_REALTIME, &time);
    
    time.tv_sec+=d_ms/1000;
    time.tv_nsec+=(d_ms % 1000)*1000000;

    if(time.tv_nsec > milliard){
      time.tv_sec+=1;
    }
    time.tv_nsec%=milliard;
    print_timestamp();
    debug_printf("Thread %u going to sleep for %u",id,d_ms);
    debug_printf(" Should be back at ");
    print_timespec(time);
    debug_printf ("\n");

    threads[id].awaken_date = time ;
    threads[id].state = IDLE;
    
    setup_timer(d_ms,false);

    //we need to reschedule right now
    scheduler();
  }
}
