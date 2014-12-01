#ifndef __WORKLOAD_H__
#define __WORKLOAD_H__

/******************************************************************************/
void initialize_period (void);
/* Initialize the base period used to simulate a workload                     */

void compute_during_n_times_100ms (int n);
/* Simulate (really approximatively) a workload for n times 100 ms            */

/******************************************************************************/
#endif /* __WORKLOAD_H__ */
