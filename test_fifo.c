#include<stdlib.h>

#include "constants.h"
#include "debug.h"
#include "um_threads.h"
#include "fifo.h"
#include "user_code.h"
#include "workload.h"

/******************************************************************************/
/* Main entry point: configure a FIFO within priority scheduler               */

int main (int argc, char **argv) {
  int i;
  um_thread_id tid;

  initialize_period();               /* initialize workload simulator         */

  TODO(Exercise 4)

  start_scheduler();                 /* force a swap to the first context     */
  
  return EXIT_SUCCESS; 
}
