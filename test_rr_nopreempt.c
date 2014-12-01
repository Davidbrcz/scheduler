#include<stdlib.h>

#include "constants.h"
#include "debug.h"
#include "um_threads.h"
#include "rr.h"
#include "user_code.h"
#include "workload.h"

/******************************************************************************/
/* Main entry point: configure a non-preemptive round-robin scheduler         */

int main (int argc, char **argv) {
  int i;
  um_thread_id tid;

  initialize_period();               /* initialize workload simulator         */
  configure_rr_scheduler(0);         /* configure round robin scheduler       */

  for (i=0; i < MAX_THREADS; i++)    /* define several threads                */
    tid = um_thread_create(user_thread_no_preempt, 0, i);
  
  start_scheduler();                 /* force a swap to the first context     */
  
  return EXIT_SUCCESS;               /* we should never reach this point      */
}
