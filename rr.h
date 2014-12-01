/* This file defines a Round Robin scheduler to be integrated to the
 * um_threads library 
 */

#ifndef __ROUND_ROBIN_H__
#define __ROUND_ROBIN_H__

#include<stdint.h>
#include "um_threads.h"

/******************************************************************************/
void configure_rr_scheduler (uint32_t period);
/* Configure the Round Robin Scheduler, 
 * if period is non null, implement a preemptive RR policy.
 */

um_thread_id scheduler_rr (void);
/* Round robin scheduling policy, works for both the preemptive and
 * non preemptive cases.
 */

/******************************************************************************/
#endif /* __ROUND_ROBIN_H__ */
