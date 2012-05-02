#include "3140_finalproject.h"

typedef struct msp430_impl{
 	int ID;
 	char *message;
 	int chat_IDs[20];
 	msp430_obj *signal_next;
 	int state;
 }msp430_obj; 
extern msp430_obj *root;



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

int main(void){
	int found_id;
	int array_size;
	char *id_str;
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
		// If id was found and state is in chat mode
		if (found_id && root->state == 2){	
			uart_puts("Message From ID: ");
			//char *id;
			sprintf(id_str,"%i",root->ID);
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
		}else if (root->state == 0){
			uart_puts("Located ID: \n");
			sprintf(id_str,"%i",root->ID);
			uart_puts(id_str);
			uart_puts("\n");
		}
	
	
	}//end program loop
	
	
	
	

	
	return 0;
}
}