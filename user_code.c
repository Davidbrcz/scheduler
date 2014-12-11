#include "debug.h"
#include "um_threads.h"
#include "user_code.h"
#include "workload.h"

/******************************************************************************/
/* User code to be executed by each thread                                    */

void user_thread(void) {
  int i;
  um_thread_id my_id = get_current_context_id();

  debug_printf ("Starting thread %d\n", my_id);

  while (1) {
    for (i = 0; i< 5; i++) {
      debug_printf ("* %d\n", my_id);
      compute_during_n_times_100ms (1);
    }
  }
}
void user_thread_no_preempt(void) {
  int i;
  um_thread_id my_id = get_current_context_id();

  debug_printf ("Starting thread no preempt %d\n", my_id);

  while (1) {
    for (i = 0; i< 5; i++) {
      debug_printf ("* %d\n", my_id);
      compute_during_n_times_100ms (1);
    }
    um_thread_yield ();
  }
}

void user_thread_delay(void) {
  int i;
  um_thread_id my_id = get_current_context_id();

  debug_printf ("Starting thread delay %d\n", my_id);

  while (1) {
    for (i = 0; i< 5; i++) {
      debug_printf ("* %d\n", my_id);
      compute_during_n_times_100ms(1);
    }
    um_delay(my_id*300);
  }
}
