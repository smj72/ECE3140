#include "3140_finalproject.h"

char *id_str;


/* Parameterized "sleep" helper function */
void sleep(unsigned long count) {
	int i;
	for (i = 0; i < 10; i++) {
		while(count > 0) {
			count--;
			__no_operation();
		}
	}
}
void interface_loop(void){
	
	//char buffer[10] = {0};
	
	
	//char *id_str = &buffer[0];
	int found_id;
	int array_size;
	int quit;
	
	while(1){
		// Get head of messages received
		if(root->state == CHAT_MODE){
			msp430_obj* head = root->signal_next;
			
			// Pop off top message and print out the message
			if( head != NULL){
				root->signal_next = head->signal_next;
				head->signal_next = NULL;
				
				if(root->chat_want_ID == head->ID){
					found_id = 1;
				}
				else
				{
					found_id = 0;
				}
			
				// If id was found and state is in chat mode
				if (found_id && root->state == CHAT_MODE){	
					uart_puts("\nMessage From ID: \n");
					sprintf(id_str,"%d: ",head->ID);
					uart_puts(id_str);
					uart_puts(head->message);
				
					uart_puts("\n\nResponse: \n");
					root->message = "\0";
						
				}
			}
		}
	
	//__bis_SR_register(LPM3_bits + GIE);       // Enter LPM3, interrupts enabled 
	sleep(10000);
	}//end program loop
	
}

int main(void){
	
	WDTCTL = WDTPW + WDTHOLD;
 	P1DIR = 0x03;
 	P1OUT = 0x00;
	id_str = (char *) malloc(10);
	
	init_uart();
	uart_clear_screen();
	uart_puts("\nUart initiated for MSP430 Chat interface.\n");
	
	BSP_Init();
	
	/* Initialize minimal RF interface, wake up radio */
	MRFI_Init();
	MRFI_WakeUp();
	
	
	/* Perform board-specific initialization */
	
	receive_message();
			
	uart_puts("\nRadio initialized.\n");
	uart_puts("Starting interface.\n");
	uart_puts("Please type in the ID number you would like to have. \n");
	
	interface_loop();
	
	return 0;
}
