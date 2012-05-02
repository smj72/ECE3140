#include "3140_finalproject.h"
#include<stdio.h>
#include<string.h>

typedef struct msp430_impl{
 	int ID;
 	char *message;
 	int chat_IDs[20];
 	msp430_obj *signal_next;
 	int state;
 }msp430_obj; 
extern msp430_obj *root;

#define NETWORK_MODE 0x00
#define CHAT_ACCEPT_MODE 0x01
#define CHAT_MODE 0x02
/* Parameterized "sleep" helper function */
void sleep(unsigned int count) {
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
	char *id_str = (char*)malloc(sizeof(char));
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
		
		// If there are chat ids, make sure the message sender is part of the ids
		array_size = sizeof(root->chat_IDs)/sizeof(int);
		found_id = 0;
		if (array_size != 0){
			int i = 0;
			for(i;i<array_size;i++){
				if(root->chat_IDs[i] == head->ID){
					found_id = 1;
					break;
				}
			}
		}
	}
		// If id was found and state is in chat mode
		if (found_id && root->state == CHAT_MODE){	
			uart_puts("Message From ID: ");
			//char *id;
			sprintf(id_str,"%d",root->ID);
			uart_puts(id_str);
			uart_puts("\n");
			uart_puts(head->message);
			// If message quit then remove from chat ids
			quit = 0;
			if (head->message == "quit"){
				int i = 0;
				for(i;i<array_size;i++){
					if(root->chat_IDs[i] == head->ID){
						root->chat_IDs[i] = NULL;
						quit = 1;
						break;
					}
				}
			}
			
			
			// If there was no quit message
			if (!quit){
				uart_puts("Respond To (enter id): ");
				root->message = NULL;
				// busy wait for message
				while(root->message == NULL);
		
				uart_puts("Enter message: \n");
				root->message = NULL;
				// busy wait for message
				while(root->message[0] == NULL);
		
				// SEND RADIO SIGNAL
			}
		// NETWORK STATE
		}
		else if (root->state == NETWORK_MODE){
			if(root->message!=NULL&& strcmp (root->message,"\0") != 0){
				uart_puts(root->message);
				send_message();
				root->message = "\0";
			}
			/*uart_puts("Located ID: \n");

			sprintf(id_str,"%d",root->ID);
			uart_puts(id_str);
			uart_puts("\n");*/
			
			//uart_puts("\nLooking for other MSP430s...\n");
			sleep(5000000);
			
		}
	
	sleep(10000);
	}//end program loop
	
}

int main(void){
	
	WDTCTL = WDTPW + WDTHOLD;
 	P1DIR = 0x03;
 	P1OUT = 0x00;
 	
 	
 	
 	
	
	
	init_uart();
	uart_clear_screen();
	uart_puts("\nUart initiated for MSP430 Chat interface.\n");
	
	//init_radio();
	
	BSP_Init();
	
	/* Initialize minimal RF interface, wake up radio */
	MRFI_Init();
	MRFI_WakeUp();
	
	
	/* Perform board-specific initialization */
	
	receive_message();
	
			
	
	uart_puts("\nRadio initialized.\n");
	
	
	uart_puts("\nStarting interface.\n");
	
	interface_loop();
	
	
	
	//process_start();

	
	
	
	

	
	return 0;
}
