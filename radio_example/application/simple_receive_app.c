/*------------------------------------------------------------
 * Example radio "receive" application
 *   Always blinks green LED.
 *   Toggles red LED every time a packet is received.
 *----------------------------------------------------------*/
#include "3140_finalproject.h"
char packet_message[20];
msp430_obj *senders;

//Initialization for receiving messages
void receive_message(void){
	
	/* Filter setup return value: success or failure */
	unsigned char status;
	
	/* Set a filter address for packets received by the radio
	 *   This should match the "destination" address of
	 *   the packets sent by the transmitter. */
	uint8_t address[] = {0x12,0x34,0xac,0xcd};
	senders = (msp430_obj*)malloc(sizeof(msp430_obj));
	
	/* Attempt to turn on address filtering
	 *   If unsuccessful, turn on both LEDs and wait forever */
	status = MRFI_SetRxAddrFilter(address);	
	MRFI_EnableRxAddrFilter();
	if( status != 0) {
		P1OUT = RED_RECEIVE_LED | GREEN_LED;
		while(1);
	}
		
	
	/* Turn on the radio receiver */
	MRFI_RxOn();
	
	
	__bis_SR_register(GIE);

//Green LED signals radio receiver is on and can obtain messages
	P1OUT = GREEN_LED;
}


/* Function to execute upon receiving a packet
 *   Called by the driver when new packet arrives */
void MRFI_RxCompleteISR(void) {
	/* Read the received data packet */
	int sender_id;
	int sender_want_chat_id;
	
	//char * packet_message;
	
	mrfiPacket_t	packet;
	MRFI_Receive(&packet);
	
	sender_id = packet.frame[9];
	sender_want_chat_id = packet.frame[10];
	
	//Accept package if ID matches
	if(sender_want_chat_id == 0 || sender_want_chat_id == root->ID){
		
		//packet_message = (char *) malloc(2);
		/*uart_puts("\nRadio Message Received!\n");
		uart_puts("sender: ");
		sprintf(packet_message,"%d",sender_id);
		uart_puts(packet_message);
		uart_puts("\nmessage: ");
		sprintf(packet_message,"%s", &packet.frame[11]);
		uart_puts(packet_message);
		uart_putc('\n');*/
		
		
		//Conditions for chat mode. chat_ID = 0 means it will accept any chat request
		if(root->state == CHAT_ACCEPT_MODE && (root->chat_IDs[0] == sender_id || root->chat_IDs[0]==0))
		{
			int chat_accept_id = packet_message[0] - '0';
			if(chat_accept_id == root->ID){
				root->chat_IDs[0] = chat_accept_id;
				memset(&packet_message[0], 0, sizeof(packet_message));
				sprintf(packet_message,"%d",root->ID);
				send_message(packet_message);
				root->state = CHAT_MODE;
				uart_puts("\nChat initiated\n");
				//clear msp430 list
				root->signal_next = NULL;
			}
			else{
				root->state = NETWORK_MODE;
				uart_puts ("Chat rejected.\nPlease type in another ID\n");
			}
			
			
		}
		//Add msp430 to linked list in network/chat accept modes
		else if(root->state == NETWORK_MODE || root->state == CHAT_ACCEPT_MODE){
			
			senders = root;
			while(senders)
			{
				//Only add to linked list if it's a new msp430
				if(senders->ID == sender_id){
					break;
				}
				else if(senders->signal_next == NULL){
					msp430_obj *new_msp430 = (msp430_obj*) malloc(sizeof(msp430_obj));
					
					new_msp430->ID = sender_id;
					new_msp430->chat_IDs[0]=sender_want_chat_id;
					new_msp430->state = NETWORK_MODE;
					new_msp430->signal_next = root->signal_next;
					
					root->signal_next = new_msp430;
					
					//Update list of available msp430s to chat with
					uart_puts("\n These msp430s wish to chat with you:\n");
					senders = root->signal_next;
					memset(&packet_message[0], 0, sizeof(packet_message));
					while(senders!=NULL){
						sprintf(packet_message,"%d ",senders->ID);
						uart_puts(packet_message);
						senders = senders->signal_next;
					}
					uart_putc('\n');
					
					break;
				}
				
				senders = senders->signal_next;
			}
			
			//free(senders);
		}
		//If in chat mode, check ID and automatically add to stack
		else if(root->state == CHAT_MODE && sender_id == root->chat_IDs[0])
		{
			msp430_obj *new_msp430 = (msp430_obj*) malloc(sizeof(msp430_obj));
			
			
			new_msp430->ID = sender_id;
			new_msp430->chat_IDs[0]=sender_want_chat_id;
			new_msp430->state = NETWORK_MODE;
			new_msp430->signal_next = root->signal_next;
			new_msp430->message = (char *) &packet.frame[11];
			
			root->signal_next = new_msp430;
		}
		
	}
	else{
		uart_puts("\nDEBUG: Message Blocked\n");
		
	}
	//free(packet_message);
	
	
	/* Toggle the red GREEN to signal that data has arrived */
	
	P1OUT ^= GREEN_LED;
	sleep(60000);
	P1OUT ^= GREEN_LED;
	__bis_SR_register(GIE);
}


