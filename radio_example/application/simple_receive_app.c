/*------------------------------------------------------------
 * Example radio "receive" application
 *   Always blinks green LED.
 *   Toggles red LED every time a packet is received.
 *----------------------------------------------------------*/
#include "3140_finalproject.h"
char packet_message[CHAR_LIMIT];
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
	 *   If unsuccessful, turn on both LEDs and wait forever *
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
	//clear low power mode
	//__bic_SR_register(LPM3_bits);
	
	sender_id = packet.frame[9];
	sender_want_chat_id = packet.frame[10];
	
	//Commands
		//Find: Send back response command
		//strcpy(  packet_message, (char *) &packet.frame[11]  );
		if(strncmp((char*) &packet.frame[11], "/find", 5)==0){
			send_message("/response");
		}
		if(strncmp((char*) &packet.frame[11], "/response", 9)==0){
			memset(&packet_message[0], 0, sizeof(packet_message));
			sprintf(packet_message,"\nID [%d] located.\n",sender_id);
			uart_puts(packet_message);
		}
	
	//Accept package if this MSP430 accepts anything or a package was meant for you
	if(root->chat_want_ID == -1 || sender_want_chat_id == root->ID){
		
		
		//Conditions for chat mode. chat_ID = 0 means it will accept any chat request
		
		//Add msp430 to linked list in network/chat accept modes
		if(root->state == NETWORK_MODE ){
			if (root->chat_want_ID == sender_id || root->chat_want_ID == -1)
			{
				root->chat_want_ID = sender_id;
				
				memset(&packet_message[0], 0, sizeof(packet_message));
				sprintf(packet_message,"\n %d chat accept\n",root->ID);
				
				send_message(packet_message);
				root->state = CHAT_MODE;
				uart_puts("\nChat initiated\n");
				
				//clear msp430 list
				root->signal_next = NULL;
				
			}
			//else notify msp430 of incoming chat request
			else{
					/*uart_puts("\n This msp430s wishes to chat with you:\n");
					memset(&packet_message[0], 0, sizeof(packet_message));
					sprintf(packet_message," %d ",sender_id);
					uart_puts(packet_message);
					uart_putc('\n');*/
				senders = root;
				while(senders)
				{
					
					if(senders->signal_next == NULL){
						msp430_obj *new_msp430 = (msp430_obj*) malloc(sizeof(msp430_obj));
						
						new_msp430->ID = sender_id;
						new_msp430->chat_want_ID=sender_want_chat_id;
						new_msp430->state = NETWORK_MODE;
						new_msp430->signal_next = root->signal_next;
						
						root->signal_next = new_msp430;
						//root->state = CHAT_ACCEPT_MODE;
						
						//Update list of available msp430s to chat with
						uart_puts("\n These msp430s wish to chat with you:\n");
						senders = root->signal_next;
						memset(&packet_message[0], 0, sizeof(packet_message));
						while(senders!=NULL){
							sprintf(packet_message," %d ",senders->ID);
							uart_puts(packet_message);
							senders = senders->signal_next;
						}
						uart_putc('\n');
						
						break;
						
					}
					//Only add to linked list if it's a new msp430
					else if(senders->signal_next->ID == sender_id){
						//If he wants to be removed
						
						if(strncmp((char*) &packet.frame[11], "/remove", 7)==0){
							msp430_obj *remove = senders->signal_next;
							memset(&packet_message[0], 0, sizeof(packet_message));
							sprintf(packet_message,"\n%d does not want to chat anymore\n",senders->ID);
							senders->signal_next = senders->signal_next->signal_next;
							free(remove);
	
						}
						break;
					}
					
					senders = senders->signal_next;
				}
			}
		}
		//If in chat mode, check ID and automatically add to stack
		else if(root->state == CHAT_MODE && sender_id == root->chat_want_ID)
		{
			msp430_obj *new_msp430 = (msp430_obj*) malloc(sizeof(msp430_obj));
			char *msg_init = (char*)malloc(30);
			
			new_msp430->ID = sender_id;
			new_msp430->chat_want_ID=sender_want_chat_id;
			new_msp430->state = NETWORK_MODE;
			new_msp430->signal_next = root->signal_next;
			strcpy( (char *) msg_init, (char *) &packet.frame[11]  );
			new_msp430->message = msg_init;
			
			root->signal_next = new_msp430;
			uart_puts("\n message received \n");
		}
		
	}
	
	else if(root->state == NETWORK_MODE && strncmp((char*) &packet.frame[11], "/remove", 7)==0){
		senders = root;

		while(senders)
		{
			
			if(senders->signal_next->ID == sender_id){
				//If he wants to be removed
				
					msp430_obj *remove = senders->signal_next;
					memset(&packet_message[0], 0, sizeof(packet_message));
					sprintf(packet_message,"\n%d does not want to chat anymore\n",senders->ID);
					senders->signal_next = senders->signal_next->signal_next;
					free(remove);

				
				break;
			}
			
			senders = senders->signal_next;
		}
		
			
	}
	else{
		uart_puts("\n Blocked radio message \n");
	}
	
	
	/* Toggle the red GREEN to signal that data has arrived */
	
	P1OUT ^= GREEN_LED;
	sleep(1000);
	P1OUT ^= GREEN_LED;
	
}


