#ifndef __TABLE_DRIVEN_H__
#define __TABLE_DRIVEN_H__

#include<stdint.h>
#include "um_threads.h"

/******************************************************************************/
typedef struct {
  um_thread_id a_thread;
  uint32_t slot_time;
} table_entry;

typedef struct {
  uint32_t size;
  table_entry *table_entries;
} schedule_table;

void configure_table_driven_scheduler (schedule_table *table);
/* Configure the Round Robin Scheduler, 
 * if period is non null, implement a preemptive RR policy.
 */

um_thread_id scheduler_table_driven (void);
/* Round robin scheduling policy, works for both the preemptive and
 * non preemptive cases.
 */

/******************************************************************************/
#endif /* __TABLE_DRIVEN_H__ */
