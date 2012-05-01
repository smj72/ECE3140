#include "3140_finalproject.h"
#include "3140_concur.h"
#include "bsp.h"
#include "mrfi.h"
#include "radios/family1/mrfi_spi.h"

lock_t l;
msp430_obj *my_msp430_ptr = NULL;

typedef struct msp430_impl{
 	int ID;
 	char message [255];
 	msp430_obj *signal_next;
 	
 }msp430_obj;

void send_uart_input()
{


}

void init_radio(void){
	
	/* Perform board-specific initialization */
	BSP_Init();
	
	/* Initialize minimal RF interface, wake up radio */
	MRFI_Init();
	MRFI_WakeUp();
	
	l_init (&l);
	
	if (process_create (p1,10) < 0) {
	 	return -1;
	}
	if (process_create (p1,10) < 0) {
	 	return -1;
	}
	
	process_start();
	P1OUT= 0x02;
	
}