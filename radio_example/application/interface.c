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
	
	msp430_obj* head = root->signal_next;
	
	// Pop off top message and print out the message
	if(head != NULL){
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
			uart_puts("Message From ID: ");
			//char *id;
			sprintf(id_str,"%d",head->ID);
			uart_puts(id_str);
			uart_puts("\n");
			uart_puts(head->message);
			uart_puts("\n");
			// If message quit then remove from chat ids
			quit = 0;
			if (head->message == "/quit"){
				int i = 0;
				if(root->chat_want_ID == head->ID){
					root->chat_want_ID = 0;
					quit = 1;
					break;
				}
			}
			//free head memory
			free(head->message);
			free(head);
			
			
			// If there was no quit message
			if (!quit){
				uart_puts("Response: \n");
				root->message = NULL;
				// busy wait for message
				//while(root->message == NULL);
		
				//uart_puts("Enter message: \n");
				//root->message = NULL;
				// busy wait for message
				//while(root->message[0] == NULL);
		
				// SEND RADIO SIGNAL
			}
		}
		// NETWORK STATE
		}
		else if (root->state == CHAT_ACCEPT_MODE){
			if(root->message!=NULL&& strcmp (root->message,"\0") != 0){
				int id_want = root->message[0] - '0';
				root->chat_want_ID = id_want;
				//uart_puts();
				
				send_message(&(root->message[1]));
				root->message = "\0";
			}
		}
		else if (root->state == NETWORK_MODE){
			/*root->message = "smj72";
			while(1){
				send_message();
				}*/
			if(root->message!=NULL&& strcmp (root->message,"\0") != 0){
				//int id_want = root->message[0] - '0';
				int id_want = atoi(root->message);
				root->chat_want_ID = id_want;
				//uart_puts();
				
				send_message(root->message);
				root->message = "\0";
			}
		
			
			//low power mode to wait for new update
			
			
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
	uart_puts("\nStarting interface.\n");
	uart_puts("\nPlease type in the ID number you would like to have. \n");
	
	interface_loop();
	
	return 0;
}
