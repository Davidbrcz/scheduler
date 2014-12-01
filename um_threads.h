/******************************************************************************
 * User-mode thread library 
 *
 * This library relies on <ucontext.h> to define a limited runtime to
 * support threads and various scheduling policies.
 ******************************************************************************/

#ifndef __UM_THREADS_H__
#define __UM_THREADS_H__

#include "constants.h"
#include<ucontext.h>
#include<stdint.h>

/******************************************************************************/
/* Thread entities                                                            */

typedef uint32_t um_thread_id;  /* id of a thread         */
typedef uint32_t stack_size_t;  /* stack size of a thread */
typedef uint32_t priority_t;    /* priority               */

typedef enum { IDLE, READY, RUNNING } thread_state_t;

typedef struct {                /* thread control block   */
  ucontext_t     um_context;
  um_thread_id   tid;
  stack_size_t   stack_size;
  priority_t     priority;
  thread_state_t state;
} um_thread_t;

extern um_thread_t threads[MAX_THREADS];
/* Array of threads currently configured in the program */

um_thread_id um_thread_create
(void (*function)(void),  
 stack_size_t stack_size,
 priority_t priority);
/* um_thread_create: helper function to create a thread context
 * - initialize the context, 
 * - setup the new stack, 
 * - signal mask,
 * - function to execute
 */

void um_thread_yield (void);
/* um_thread_yield: relinquish CPU */

ucontext_t *get_context (um_thread_id tid);

ucontext_t *get_current_context (void);
um_thread_id get_current_context_id (void);

/******************************************************************************/
/* Scheduler                                                                  */

typedef um_thread_id (* scheduler_function) (void);

void configure_scheduler (scheduler_function s);
void start_scheduler (void);
void scheduler(void);

/******************************************************************************/
#endif /* __UM_THREADS_H__ */
