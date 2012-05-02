#include "3140_finalproject.h"

typedef struct msp430_impl{
 	int ID;
 	char *message;
 	int chat_IDs[20];
 	msp430_obj *signal_next;
 	int state;
 }msp430_obj; 

int main(void){
	int found_id;
	int array_size;
	char *id;
	int quit;
	
	while(1){
	// Get head of messages received
	msp430_obj* head = root.signal_next;
	
	// Pop off top message and print out the message
	if(head != NULL){
		root.signal_next = head->signal_next;
		head->signal_next = NULL;
		
		// If there are chat ids, make sure the message sender is part of the ids
		array_size = sizeof(root.chat_IDs)/sizeof(int);
		found_id = 0;
		if (array_size != 0){
			int i = 0;
			for(i;i<array_size;i++){
				if(root.chat_IDs[i] == head->ID){
					found_id = 1;
					break;
				}
			}
		}
		
		if (found_id){	
			uart_puts("Message From ID: ");
			//char *id;
			itoa(head->ID,id,10);
			uart_puts(id);
			uart_puts("\n");
			uart_puts(head->message);
			// If message quit then remove from chat ids
			quit = 0;
			if (head->message == "quit"){
				int i = 0;
				for(i;i<array_size;i++){
					if(root.chat_IDs[i] == head->ID){
						root.chat_IDs[i] = NULL;
						quit = 1;
						break;
					}
				}
			}
			
			
			if (!quit){
				uart_puts("Respond To (enter id): ");
				root.message = NULL;
				// busy wait for message
				while(root.message == NULL);
		
				uart_puts("Enter message: \n");
				root.message = NULL;
				// busy wait for message
				while(root.message[0] = NULL);
		
				// SEND RADIO SIGNAL
			}
		}
	
	
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	return 0;
}