#include<stdlib.h>

#include "constants.h"
#include "debug.h"
#include "um_threads.h"
#include "table_driven.h"
#include "user_code.h"
#include "workload.h"

/******************************************************************************/
/* Main entry point: configure a Table driven scheduler                       */

/******************************************************************************/
int main () {
  /* int i;
     um_thread_id tid;
  */
  initialize_period();                /* initialize workload simulator        */

  um_thread_create(&user_thread,0,128);
  um_thread_create(&user_thread,0,128);
  um_thread_create(&user_thread,0,128);

  table_entry schedule_array[] = {{1, 100},{0,200},{2,600}};
  size_t size_array=sizeof(schedule_array)/sizeof(schedule_array[0]);

  schedule_table table={size_array,schedule_array};

  configure_table_driven_scheduler(&table);
  start_scheduler();                 /* force a swap to the first context     */
  
  return EXIT_SUCCESS; 
}
