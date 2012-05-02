#include "3140_finalproject.h"
#include "3140_concur.h"
#include "bsp.h"
#include "mrfi.h"
#include "radios/family1/mrfi_spi.h"

lock_t l;
msp430_obj *my_msp430_ptr = NULL;

/*typedef struct msp430_impl{
 	int ID;
 	char message [255];
 	msp430_obj *signal_next;
 	
 }msp430_obj;*/

void send_uart_input()
{


}

void init_radio(void){
	msp430_obj *dummy = (msp430_obj*)malloc(sizeof(msp430_obj));
	char *dummy_msg = (char*)malloc(sizeof(char));
	WDTCTL = WDTPW + WDTHOLD;
 	P1DIR = 0x03;
 	P1OUT = 0x00;
 	
 	
 	dummy->ID = 1;
 	dummy->message = dummy_msg;
 	dummy->signal_next = NULL;
 	
 	
	BSP_Init();
	
	/* Initialize minimal RF interface, wake up radio */
	MRFI_Init();
	MRFI_WakeUp();
	
	l_init (&l);
	
	init_uart(dummy);	
 	uart_clear_screen();
	/* Perform board-specific initialization */
	
	if (process_create (receive_message(dummy),10) < 0) {
	 	return -1;
	}
	if(my_msp430.message !=NULL){
		if (process_create (send_message(dummy),10) < 0) {
	 	return -1;
	}
			
	}
	
	
	process_start();
	P1OUT= 0x02;
	
}