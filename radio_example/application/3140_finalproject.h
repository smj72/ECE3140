/*bleh*/
#ifndef __3140_FINALPROJECT_H__
#define __3140_FINALPROJECT_H__

#include "msp430x22x4.h"
#include <stdlib.h>

#include "bsp.h"
#include "mrfi.h"
#include "radios/family1/mrfi_spi.h"
#include <string.h>
#include<stdio.h>
#include <ctype.h>

/* Useful #defines */
#define RED_SEND_LED 		0x01
#define RED_RECEIVE_LED     0x01
#define GREEN_LED   		0x02
#define NETWORK_MODE 		0x00
#define CHAT_ACCEPT_MODE	0x01
#define CHAT_MODE 			0x02
#define CHOOSE_ID_MODE		0x03
#ifndef NULL
#define NULL 0
#endif

/*---------------------------------------------------------------------------------------
  
The following defines the msp430 structure in order to help maintain many signals at once
Each msp 430 has a linked list of other msp430s that it can connect with.

Each root represents its own msp430. Each child msp430's next is connected with the root

---------------------------------------------------------------------------------------*/
 
typedef struct msp430_impl msp430_obj; 
//struct msp430_obj;
struct msp430_impl{
 	int ID;
 	char *message;
 	int chat_IDs[4];
 	msp430_obj *signal_next;
 	int state;
 };

extern msp430_obj *root;

/*------------------------------------------------------------------------
  
The following functions are for use in UART. They provide the host a way to communicate with the
MCU using a serial terminal.

------------------------------------------------------------------------*/

/* This variable maintains what the user typed in */
extern char out[20];

/* Sets up uart bits */
void init_uart (void);

/* Print a character in terminal */
void uart_putc (char c);

/* Print a string in terminal */
void uart_puts (char *str);

/* Clears screen */
void uart_clear_screen (void);

void set_msp430(msp430_obj msp430);

/*------------------------------------------------------------------------
  
The following functions are for use in Radio Transmission. 

------------------------------------------------------------------------*/

int init_radio(void);

void MRFI_RxCompleteISR(void);

//char* send_message(char *msg);

void send_message (char *msg);

//void receive_message(void);

void receive_message(void);

void sleep(unsigned int count);

#endif /*__3140_FINALPROJECT_H__*/
