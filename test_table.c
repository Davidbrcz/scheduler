#include<stdlib.h>

#include "constants.h"
#include "debug.h"
#include "um_threads.h"
#include "table_driven.h"
#include "user_code.h"
#include "workload.h"

/******************************************************************************/
/* Main entry point: configure a Table driven scheduler                       */

  TODO(Exercise 3)

/******************************************************************************/
int main (int argc, char **argv) {
  int i;
  um_thread_id tid;

  initialize_period();                /* initialize workload simulator        */

  TODO(Exercise 3)
  start_scheduler();                 /* force a swap to the first context     */
  
  return EXIT_SUCCESS; 
}
