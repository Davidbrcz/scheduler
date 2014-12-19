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
/*  int i;
  um_thread_id tid;*/

  initialize_period();               /* initialize workload simulator         */

  um_thread_create(&user_thread_delay,0,100);
  um_thread_create(&user_thread_delay,0,255);
  um_thread_create(&user_thread_delay,0,128);
  um_thread_create(&user_thread_delay,0,200);


  configure_fifo_scheduler();
  start_scheduler();                 /* force a swap to the first context     */
  
  return EXIT_SUCCESS; 
}
