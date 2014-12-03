#include<time.h>
#include "debug.h"

/******************************************************************************/
void print_timestamp (void) {
  struct timespec tp;
  clock_gettime (CLOCK_REALTIME, &tp);

  print_timespec(tp);
}

void print_timespec(struct timespec tp){
  debug_printf  ("[%d.%ld] ", (int) tp.tv_sec % 1000, tp.tv_nsec/CLOCKS_PER_SEC);
}
/******************************************************************************/
