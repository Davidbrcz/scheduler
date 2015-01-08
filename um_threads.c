#include<signal.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/time.h>

#include "constants.h"
#include "debug.h"
#include "um_threads.h"
#include "timer_interrupt.h"
#include <time.h>
/******************************************************************************/
um_thread_t threads[MAX_THREADS];

uint32_t um_thread_index = 0;   

int sched_current_context_id = 0;  /* Current thread executing                */
ucontext_t *sched_context;         /* a pointer to the running thread         */

scheduler_function the_scheduler;

const struct timespec infinite_time = {0,0};
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
  threads[um_thread_index].awaken_date =  infinite_time;
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
  debug_printf("-----\n");
  um_thread_id previous = sched_current_context_id;

  /* 
     if the thread is not waiting something, the we move 
     it back from RUNNING to READY. Update for question 5.
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
  debug_printf("-----\n");
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

void compute_awaken_time(struct timespec* time,uint32_t d_ms)
{
   time->tv_sec+=d_ms/1000;
    time->tv_nsec+=(d_ms % 1000)*1000000;

    if(time->tv_nsec > milliard){
      time->tv_sec+=1;
    }

    time->tv_nsec%=milliard;
}
/*Compute duration between t1 and t0 in us. t1 is after t0*/
long long difftime_timespec(struct timespec t1,struct timespec t0){

  long long diff = (t1.tv_nsec - t0.tv_nsec)/1000;
  if(t1.tv_sec > t0.tv_sec){
    diff+=1000*1000;
  }

  return diff;
}


bool valid_awaken_date(struct timespec t){

  if(t.tv_sec == infinite_time.tv_nsec && t.tv_nsec == infinite_time.tv_nsec) {
    return false;
  }
  return true;
}

/*
  True it t0 is before t1 (t0<t1)
*/
bool timespec_lowereq_than(struct timespec t0,struct timespec t1){

  //handle cases where t1 is infinite_time
  //then t0 is always before t1
  if(!valid_awaken_date(t1)){
    return true;
  }
  //handle cases where t0 is infinite_time
  //then t0 is NOT before t1
  if(!valid_awaken_date(t0)){
    return false;
  }

  //handle regular cases
  if(t0.tv_sec < t1.tv_sec) {
    return true;
  }else if(t0.tv_sec > t1.tv_sec){
    return false;
  }else { /*same second*/
    if(t0.tv_nsec <= t1.tv_nsec) {
      return true;
    }else if(t0.tv_nsec > t1.tv_nsec){
      return false;
    }
  }
}

bool is_same_date(struct timespec t1,struct timespec t2){
  long s1 = t1.tv_sec % 1000;
  long ms1 = t1.tv_nsec/CLOCKS_PER_SEC;

  long s2 = t2.tv_sec % 1000;
  long ms2 = t2.tv_nsec/CLOCKS_PER_SEC;

  return s1==s2 && ms1==ms2;
}


/*
  This function is called from scheduler_fifo when there is a least a thread that is 
  sleepy . It means there is at least an IDLE thread
*/
void timer_lowest_awken_date(){

  size_t i = 0;
  struct timespec time = infinite_time;

  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);


  int last_i = -1;
  for (i = 0; i < um_thread_index; ++i)
    {
      if(!valid_awaken_date(threads[i].awaken_date)) {
	debug_printf("@@ thread %zu not legit for wake up \n",i);
	continue;
      }

      if(timespec_lowereq_than(threads[i].awaken_date,time)){
	time=threads[i].awaken_date;
	debug_printf("@@ thread %zu wakes before %d \n",i,last_i);
	last_i=i;
      }else{
	  debug_printf("@@ thread %zu does NOT wake before %d \n",i,last_i);
      }
    }

  long long delay = difftime_timespec(time,current_time) / 1000;
  debug_printf("Timer setup for thread %zu and %lld ms  ",last_i,delay);
  compute_awaken_time(&current_time,delay); 
  print_timespec(current_time);
  debug_printf ("\n");

  setup_timer(delay,false);
}

/*
  This function will register that the current thread is asking to sleep and compute its awaken time
  d_ms : How long (in ms) the current thread wants to sleep. d_ms must be < 999 
*/
void um_delay(uint32_t d_ms){
  if(d_ms==0) {
    return;
  }
  
  //stop the timer ??
  setup_timer(0,false);
  um_thread_id id = get_current_context_id();

  struct timespec time;
  clock_gettime(CLOCK_REALTIME, &time);

  compute_awaken_time(&time,d_ms); 
  /*******/
  print_timestamp();
  debug_printf("Thread %u going to sleep for %u",id,d_ms);
  debug_printf(" Should be back at ");
  print_timespec(time);
  debug_printf ("\n");
  /*******/

  threads[id].awaken_date = time ;
  threads[id].state = IDLE;

  //we need to reschedule right now
  scheduler();
}
