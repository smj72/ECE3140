#include "3140_concur.h"
#include <stdlib.h>	

// Define Locked and Unlocked variables for code readability 
#ifndef LOCKED
#define LOCKED 2
#endif

#ifndef UNLOCKED
#define UNLOCKED 1
#endif

#ifndef WAITING
#define WAITING 3
#endif



// the currently running process 
process_t* current_process = NULL;
// the head of the linked list
process_t* head = NULL;
// the lock holder (can be modified to fit any lock), called before process blocked to identify
// the holder of the lock
process_t* holder = NULL;

//These are modified to fit for any condition variable
//Front of waiting queue for condition variable
process_t* waiter_first = NULL;
process_t* waiter_curr = NULL;


//Initialization of realtime variable
//corresponds to the elapsed time on the MSP430 in seconds and milliseconds
realtime_t current_time;


//Current realtime process (the current is always the head for realtime)
process_t* realtime_current = NULL;

//Realtime queue, waiting before it is able to begin
process_t* realtime_queue = NULL;

int add_realtime = 0;

//Keep track of the number of missed deadlines
int process_deadline_miss = 0;




/*************************************************************/
/* Structure that keeps track of its process's stack pointer,*/
/* a pointer to the next process, and a locked status 		 */
/* ***********************************************************/
typedef struct process_state{
	unsigned int sp;
	unsigned int locked;
	process_t *next;
	unsigned int start_time;
	unsigned int work_time;
	unsigned int deadline_time;
}process_t;

void l_init (lock_t *l){
	// Initialize lock with no one being the holder
	l->holder = NULL;
}

void c_init (lock_t *l, cond_t *c){
	//Initialize condition variable
	c->waiter = NULL;
	c->waiter_curr = NULL;
}

void l_lock (lock_t *l){
	 //__disable_interrupt();
	 // If nobody has the lock, add the current process as the holder of the lock
	if (l->holder == NULL){
		process_t *temp1 = (process_t*)malloc(sizeof(process_t));
		// Since it is the holder of the lock, the process can continue to be unlocked
		current_process->locked = UNLOCKED;
		// Create new process_t to put into holder (and make a queue out of it)

		temp1->sp = current_process->sp;
		temp1->next = NULL;
		temp1->locked = UNLOCKED;
		
		// add it to holder
		l->holder = temp1;
		holder = temp1;
	}else{
	 // If the lock has a holder, change the current_process to indicate that its locked.
	 // Call process_blocked() to remove this process from the ready queue and add it to the
	 // locked processes corresponding to this lock
		current_process->locked = LOCKED;
		holder = l->holder;
		//__disable_interrupt();
		process_blocked();
	}
	//__enable_interrupt();	
}



void l_unlock (lock_t *l){
	process_t *temp = l->holder; 
	//__disable_interrupt();
	//if (l->holder == current_process){
		current_process->locked = UNLOCKED;

		while(temp->next->locked ==UNLOCKED){
			temp = temp->next;
		}
		// Check to see if there are any other processes waiting for the lock. If there aren't, the process
		// can carry on and the holder of the lock is no one.
		if (temp->next != NULL){
			process_t *tmp = head;
			process_t *temp1 = (process_t*)malloc(sizeof(process_t));
			// Remove the current holder and make the new holder be the next process waiting for the lock
			l->holder = temp->next;
			l->holder->locked = UNLOCKED;
			free (temp);
	
			// Make a new process node with the new holder and Add it to the end of the ready queue

			while(tmp->next != NULL && tmp->next != head){
				tmp = tmp->next;
			}

			temp1->sp = l->holder->sp;
			temp1->next = NULL;
			temp1->locked = UNLOCKED;
			tmp->next = temp1;
		}else{
			l->holder = NULL;
		}
		
	//}
	//__enable_interrupt();
}

void c_wait (lock_t *l, cond_t *c){
	process_t *temp1 = (process_t*)malloc(sizeof(process_t));
	l->holder = l->holder->next;
	//__disable_interrupt();
	
	//Relinquishes the lock but ends up in a waiting state.
	current_process->locked = WAITING;
	
	

		temp1->sp = current_process->sp;
		temp1->next = NULL;
		temp1->locked = WAITING;
		if(c->waiter ==NULL){
			
			c->waiter = temp1;
		}
		waiter_first = c->waiter;
		c->waiter_curr = temp1;
		waiter_curr = temp1;
		
		// Check to see if there are any other processes waiting for the lock. If there aren't, the process
		// can carry on and the holder of the lock is no one.
		
		// Call process_blocked() to remove this process from the ready queue and add it to the
	 // locked processes corresponding to this lock
		process_blocked();
		
	//__enable_interrupt();
}

void c_signal (lock_t *l, cond_t *c){
	process_t *temp1 = (process_t*)malloc(sizeof(process_t));
	process_t *temp2 = (process_t*)malloc(sizeof(process_t));
	process_t *tmp = head;
	process_t  *ctemp = c->waiter;
	//__disable_interrupt();
	c->waiter->locked = UNLOCKED;
	waiter_first = c->waiter->next;
	
	
	//Put signalled node back to the end of the ready queue
	while(tmp->next != NULL && tmp->next != head){
				tmp = tmp->next;
			}
	
	temp1->sp = c->waiter->sp;
	temp1->next = NULL;
	temp1->locked = UNLOCKED;
	tmp->next = temp1;
	
	
	//Reacquire the lock
	temp2->sp = c->waiter->sp;
	temp2->next = l->holder->next;
	temp2->locked = UNLOCKED;
	l->holder = temp2;
	holder = temp2;
	
	
	c->waiter = ctemp->next;
	
	//__enable_interrupt();
}

//Returns true if there is a condition variable currently waiting.
int c_waiting (lock_t *l, cond_t *c){
	return c->waiter != NULL;
}

/************************/
/* Create a new process */
/************************/
int process_create (void (*f)(void), int n){
	// Initialize process allocation in memory and retrieve the stack pointer
	int sp = process_init(f,n);
	//__disable_interrupt();

	
	// If there wasn't enough memory for the process stack to initialize, return.
	// Else create a new process node to add to the ready linked list.
	if (sp == NULL){
		return -1;
	}else{
		// Create new process_t with desired process's stack pointer
		process_t *temp1;
		temp1 = (process_t*)malloc(sizeof(process_t));
		// Make sure there is memory for this node
		if (temp1 == NULL){
			return -1;
		}
		temp1->sp = sp;
		temp1->next = NULL;
		temp1->locked = UNLOCKED;
		// Check to see if the head of the linked list has been initialized and 
		// initialize it to temp1 if it hasnt. Otherwise traverse the linked list and
		// add new node to the end.
		if (head == NULL){
			head = temp1;
		}else{
			process_t *tmp;
			tmp = head;
			while(tmp->next){
				tmp = tmp->next;
			}
			tmp->next = temp1;
		}
		return 0;
	}
}


/*********************************/
/* Create a new realtime process */
/*********************************/

/*
 * This creates a new process that arrives after start milliseconds have elapsed. 
 * The task requires work milliseconds to complete (this is the estimate of the worst-case execution time), 
 * and has a relative deadline of deadline milliseconds. 
 * n is still the stack size for the task.*/

process_rt_create(void (*f)(void), int n, realtime_t *start, realtime_t *work, realtime_t *deadline){
	// Initialize process allocation in memory and retrieve the stack pointer
	int sp = process_init(f,n);
	int start_time = start->msec + 1000*start->sec;
	int work_time = work->msec + 1000*work->sec;
	int deadline_time = deadline->msec + 1000*deadline->sec;

	
	
	
	// If there wasn't enough memory for the process stack to initialize, return.
	// Else create a new process node to add to the real-time waiting linked list.
	if (sp == NULL){
		return -1;
	}else{
		// Create new process_t with desired process's stack pointer
		process_t *temp1;
		temp1 = (process_t*)malloc(sizeof(process_t));
		// Make sure there is memory for this node
		if (temp1 == NULL){
			return -1;
		}
		temp1->sp = sp;
		temp1->next = NULL;
		temp1->locked = UNLOCKED;
		temp1->start_time = start_time;
		temp1->work_time = work_time;
		temp1->deadline_time = deadline_time;
		//Realtime waiting queue added based on start time
		if (realtime_queue == NULL){
			realtime_queue = temp1;
		}else{
			process_t *tmp;
			tmp = realtime_queue;
			if(temp1->start_time<tmp->start_time){
				temp1->next = tmp;
				realtime_queue = temp1;
			}
			else{
				while(tmp->next && tmp->next->start_time<temp1->start_time){
					tmp = tmp->next;
				}
				temp1->next = tmp->next;
				tmp->next = temp1;
			}
		}
		return 0;
	}
}
/**************************************/
/* Starts up the concurrent execution */
/**************************************/
void process_start (void){
  current_time.msec = 0;
  current_time.sec = 0;
  // Set up timer A interrupt
  TACCR0 = 3000; 
  TACCTL0 = CCIE; //enable interrupt in timer A register
  TACTL=MC_1|ID_3|TASSEL_2|TACLR; //Use up mode , divide by 8, use SMCLK to divide, reset count enable
  
  // Set up timer B interrupt
  //TBCL0 = 0xFFFF; // Maximum value for Timer B counter (needed?)
  TBCCR0 = 150;
  TBCCTL0 = CCIE; //enable interrupt in timer B register
  TBCTL=MC_1|ID_3|TBSSEL_2|TBCLR; //Use up mode , divide by 8, use SMCLK to divide, reset count enable
  //__disable_interrupt();
  
  
  // Call process_begin() to start execution of concurrent processes
  process_begin();
}

/********************************************/
/* Timer B interrupt for realtime processes */
/********************************************/

//Interrupt called for when a process' start time begins. 
//It is later added to the current running realtime processes based on deadline
#pragma vector=TIMERB0_VECTOR
__interrupt void TBC_ISR(void)
{
	int elapsed_time;
	process_t *realtime_tmp = realtime_queue;
	process_t *temp1;
	//__disable_interrupt();
	P1IFG &= ~0x04;		// clear interrupt flags
	
	
	current_time.msec++;
	if(current_time.msec >= 1000){
		current_time.sec = current_time.sec + 1;
		current_time.msec=0;
	}
	
	elapsed_time = current_time.msec + 1000*current_time.sec;
	
	while(realtime_tmp != NULL){
		
		if(realtime_tmp->start_time<=elapsed_time){
			add_realtime = 1;
			//Current realtime order based on deadline time
			if (realtime_current == NULL){
				temp1 = (process_t*)malloc(sizeof(process_t));
				temp1->sp = realtime_queue->sp;
				temp1->locked = realtime_queue->locked;
				temp1->next = NULL;
				temp1->start_time = realtime_queue->start_time;
				temp1->work_time = realtime_queue->work_time;
				temp1->deadline_time = realtime_queue->deadline_time;
				realtime_current = temp1;
				realtime_queue = realtime_queue->next;
				if(head==NULL){
					add_realtime = 3;
				}
				else{
					add_realtime = 2;
				}
			}
			
			process_blocked();
			realtime_tmp = realtime_tmp->next;
			break;
		}
		realtime_tmp = realtime_tmp->next;
		
	}
  
}



/******************************************************************/
/* Called by the runtime system to select another process.	      */
/*  "cursp" = the stack pointer for the currently running process */
/******************************************************************/
unsigned int process_select (unsigned int cursp){
	int elapsed_time;
	process_t *empty;
	process_t *temp1;
	process_t* running = current_process;
	//If nothing can currently run, end this
	
	
	
	//Current realtime order based on deadline time
	//If adding a new realtime process, add it according to deadline
	if (realtime_current != NULL){
		if((add_realtime == 2 || add_realtime == 3) && cursp != 0){
			if(current_process != NULL)
		{
			current_process->sp = cursp;
		
		}
				
			}
		if (cursp == 0){
			
			if(realtime_current->next==NULL ){
			
				//Real-time process end
				//Must get to here in order to end a real-time process
				
				//If process took past deadline time, increment miss counter
				elapsed_time = current_time.msec + 1000*current_time.sec;
				if(elapsed_time > realtime_current->deadline_time){
					process_deadline_miss++;
				}
				realtime_current = NULL;
				
				if(head==NULL && current_process == NULL && realtime_queue == NULL){
					//No processes available. End
	
					return 0;
				}
				else if (current_process != NULL){ 
					//Non-real-time process available
					return current_process->sp;
					}
				else{
					//If only a real-time process is left, wait for it to start
					__enable_interrupt();
					while(realtime_queue!=NULL);
					__disable_interrupt();
					return 0;
				}
				
			}
			else{
				realtime_current = realtime_current->next;
				return realtime_current->sp;
			}
			
		
		
		}
		if(add_realtime!=2 && add_realtime!=3)
		{realtime_current->sp = cursp;}
		
		//Add additional realtime process if one is currently running
		if(add_realtime==1){
			process_t *tmp;
			temp1 = (process_t*)malloc(sizeof(process_t));
			temp1->sp = realtime_queue->sp;
			temp1->locked = realtime_queue->locked;
			temp1->next = NULL;
			temp1->start_time = realtime_queue->start_time;
			temp1->work_time = realtime_queue->work_time;
			temp1->deadline_time = realtime_queue->deadline_time;
			realtime_queue = realtime_queue->next;

			tmp = realtime_current;
			add_realtime = 0;
			if(tmp == NULL){
				realtime_current = temp1;
				
			}
			else if(temp1->deadline_time<tmp->deadline_time){
				temp1->next = tmp;
				realtime_current = temp1;
			}
			else{
				while(tmp->next && tmp->next->deadline_time<temp1->deadline_time){
					tmp = tmp->next;
				}
				temp1->next = tmp->next;
				tmp->next = temp1;
			}
			
		}
		add_realtime = 0;
		//current_process = realtime_current;
		return realtime_current->sp;
		
	}
	//Just return if a non-realtime process hasn't even begun
	if(head==NULL){
		if(realtime_queue!=NULL){
			//If only a real-time process is left, wait for it
			__enable_interrupt();
			while(realtime_queue!=NULL);
			__disable_interrupt();
		}
		return cursp;
		
		
	}
	
	// Initialize a running pointer to keep track of terminating process
	running = current_process;
	
	// Check to see if the argument cursp is 0. This signifies no process running
	// or the current process has terminated. If its the first, the current process 
	// is set to the head of the linked list and the stack pointer is returned.
	// If a process is terminating, running is set to the current process and 
	// current process is set to null.
	if (cursp == 0){
		if (current_process == NULL){//First iteration
			current_process = head;
			return head->sp;
		}
		
		current_process = NULL;
	}
	
	if (current_process->locked == WAITING && current_process != NULL){
		process_t *temp1 = (process_t*)malloc(sizeof(process_t));
		// Iterate through condition queue and add this process to the end
		process_t *tmp = waiter_first;
		if(tmp->next==NULL && tmp == waiter_curr){
			tmp->sp=cursp;
		}
		else{	while( tmp->next!=NULL){
				tmp = tmp->next;
			}
	
			temp1->sp = cursp;
			temp1->locked = WAITING;
			temp1->next = NULL;
	
			tmp->next = temp1;
		}
		
		// delete it off the ready queue!
		
		current_process = NULL;	
	}
	
	// Check to see if the process is locked, if it is save information about it and add to the locked queue
	if (current_process->locked == LOCKED && current_process!=NULL){
		process_t *temp1 = (process_t*)malloc(sizeof(process_t));
		// Iterate through holder's queue and add this process to the end
		process_t *tmp = holder;
		if(tmp->next == NULL)
		{tmp->sp = cursp;}
		while(tmp->next){
			tmp = tmp->next;
		}
		

		temp1->sp = cursp;
		temp1->locked = LOCKED;
		temp1->next = NULL;

		tmp->next = temp1;
		
		// delete it off the ready queue!
		
		current_process = NULL;	
	}
	
	// Check to see if the current process is NULL (terminated)
	if (current_process == NULL){
		// Traverse the linked list to find the currently running process, use old 
		// to keep track of the previous node.
		process_t *curr = head;
		process_t *old = NULL;
		while(curr != running){
			old = curr;
			curr = curr->next;
		}
		// If old is still null, that means the currently running process is the head 
		// of the linked list.
		if (old == NULL){
			// Update the current process to the node after the head and check to see
			// if the there are no currently waiting process, if there are none return 0.
			// Else update the head and return the new stack pointer.
			
			free(current_process);
			current_process = head->next;
			if (head->next == NULL || head->next == head){
				head = NULL;
				if(realtime_queue!=NULL){
				//If only a real-time process is left, wait for it
				__enable_interrupt();
				while(realtime_queue!=NULL);
				__disable_interrupt();
				}
				return 0;
			}
			head = head->next;
			
			return current_process->sp;
		}else{
			
			
			//if(curr->next == NULL && curr != head)
			// Check to see if the currently running process is the last one, if it is 
			// loop it so it points to the head of the list instead of null.
			if(curr->next == NULL){
				old->next = NULL;
				free(curr);
				current_process = head;
				return current_process->sp;
			}
			// Remove the currently running process and return the new sp of the next process.
			old->next = curr->next;
			free (curr);
			current_process = old->next;
			return current_process->sp;
		}
	}


	// If the currently running process is the last node, save its stack pointer and update
	// current process to the head of the list. Return the new stack pointer. If its not the 
	// last node save the stack pointer and update current process to the next node and return
	// that new stack pointer.
	if (current_process->next == NULL){
		// There are no more so continue processing this one
		/*if (head->next == NULL){
			return head->sp;
		}*/
		current_process->sp = cursp;
		current_process = head;
		return current_process->sp;
	}else{
		current_process->sp = cursp;
		current_process = current_process->next;
		return current_process->sp;
	}
	
	
}
