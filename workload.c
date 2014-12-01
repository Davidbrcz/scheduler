#include <time.h>
#include "debug.h"

#define BASE_ITERATIONS 1000000
int multiplier = 1;

/******************************************************************************/
void compute_during_n_times_100ms (int n) {
  int i;
  float x = 1.0001;

  for (i=0; i< n * multiplier * BASE_ITERATIONS; i++)
    x = 1/x;
}

/******************************************************************************/
void initialize_period (void) {
  int i,j;
  float x = 1.0001;
  struct timespec time1, time2;

  for (j=0; j<5; j++) {
    /* We iterate 5 times to reduce cache effects */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    for (i=0; i< BASE_ITERATIONS; i++)
      x = 1/x;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
  }

  multiplier = 100000000/(time2.tv_nsec - time1.tv_nsec);
  debug_printf ("Multiplier : %d\n", multiplier);
}
