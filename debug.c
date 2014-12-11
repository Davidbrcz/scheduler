#include<time.h>
#include "debug.h"

/******************************************************************************/
void print_timestamp (void) {
  struct timespec tp;
  clock_gettime (CLOCK_REALTIME, &tp);

  print_timespec(tp);
}

void print_timespec(struct timespec tp){
  static struct timespec t0;
  static int done = 0;
  if(done == 0){
    clock_gettime(CLOCK_REALTIME, &t0);
    done=1;
  }
  debug_printf  ("[%d.%03ld] ", (int) (tp.tv_sec - t0.tv_sec) , tp.tv_nsec/CLOCKS_PER_SEC);
}
/******************************************************************************/
