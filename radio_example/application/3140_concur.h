/*************************************************************************
 *
 *  Copyright (c) 2011 Cornell University
 *  Computer Systems Laboratory
 *  Cornell University, Ithaca, NY 14853
 *  All Rights Reserved
 *
 *  $Id$
 *
 **************************************************************************
 */
#ifndef __3140_CONCUR_H__
#define __3140_CONCUR_H__

#include "msp430x22x4.h"

#ifndef NULL
#define NULL 0
#endif

typedef struct process_state process_t;
   /* opaque definition of process type; you must provide this
      implementation.
   */


/*------------------------------------------------------------------------

   THE FOLLOWING FUNCTIONS MUST BE PROVIDED.

------------------------------------------------------------------------*/

/* ====== Concurrency ====== */

unsigned int process_select (unsigned int cursp);
/* Called by the runtime system to select another process.
   "cursp" = the stack pointer for the currently running process
*/

extern process_t *current_process; 
/* the currently running process */


void process_start (void);
/* Starts up the concurrent execution */

int process_create (void (*f)(void), int n);
/* Create a new process */

/*------------------------------------------------------------------------
  
MODIFIED FOR LOCKS

------------------------------------------------------------------------*/
typedef struct lock_impl{
	process_t *holder;
}lock_t;


void l_init (lock_t *l);

void l_lock (lock_t *l);

void l_unlock (lock_t *l);

/*------------------------------------------------------------------------
  
MODIFIED FOR CONDITION VARIABLES

------------------------------------------------------------------------*/

typedef struct cond_var_impl {
  process_t *waiter;
  //Back of waiting queue for condition variable
	process_t* waiter_curr;	
} cond_t;

    void c_init (lock_t *l, cond_t *c);
    void c_wait (lock_t *l, cond_t *c);
    void c_signal (lock_t *l, cond_t *c);
    int c_waiting (lock_t *l, cond_t *c); 


/*------------------------------------------------------------------------
  
MODIFIED FOR SCHEDULING

------------------------------------------------------------------------*/

typedef struct realtime_impl{
  unsigned int sec;
  unsigned int msec;
} realtime_t;
/*------------------------------------------------------------------------
  
You may use the following functions that we have provided

------------------------------------------------------------------------*/


/* This function can ONLY BE CALLED if interrupts are disabled.
   This function switches execution to the next ready process, and is
   also the entry point for the timer interrupt.
   
   Implemented in 3140.asm
*/
void process_blocked (void);

/*
  This function is called by user code indirectly when the process
  terminates. This is handled by stack manipulation.

  Implemented in 3140.asm
  Used in 3140_concur.c
*/
extern void process_terminated (void);

/* This function can ONLY BE CALLED if interrupts are disabled. It
   does not modify interrupt flags.
*/
unsigned int process_init (void (*f)(void), int n);

/*
  This function starts the concurrency by using the timer interrupt
  context switch routine to call the first ready process.

  The function also gracefully exits once the process_select()
  function returns 0.

  This must be called with interrupts disabled.
*/
void process_begin (void);

#endif /* __3140_CONCUR_H__ */
