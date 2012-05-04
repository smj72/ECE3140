#include "3140_finalproject.h"
#include "3140_concur.h"
#include "bsp.h"
#include "mrfi.h"
#include "radios/family1/mrfi_spi.h"

lock_t l;
msp430_obj *my_msp430_ptr = NULL;

typedef struct msp430_impl{
 	int ID;
 	char *message;
 	int chat_IDs[4];
 	msp430_obj *signal_next;
 	int state;
 }msp430_obj; 

extern msp430_obj *root;

void send_uart_input()
{


}

int init_radio(void){
	
	WDTCTL = WDTPW + WDTHOLD;
 	P1DIR = 0x03;
 	P1OUT = 0x00;
 	
 	
 	
 	
	BSP_Init();
	
	/* Initialize minimal RF interface, wake up radio */
	MRFI_Init();
	MRFI_WakeUp();
	
	
	/* Perform board-specific initialization */
	
	if (process_create (receive_message,10) < 0) {
	 	return -1;
	}
	/*if(root->message !=NULL){
		if (process_create (send_message,10) < 0) {
	 	return -1;
	}
			
	}*/
	
	
	//process_start();
	//P1OUT= 0x02;
	return 1;
	
}