/*------------------------------------------------------------
 * Example radio "receive" application
 *   Always blinks green LED.
 *   Toggles red LED every time a packet is received.
 *----------------------------------------------------------*/

#include "bsp.h"
#include "mrfi.h"
#include "radios/family1/mrfi_spi.h"
#include "3140_finalproject.h"
#include <string.h>
#include<stdio.h>

/* Useful #defines */
#define RED_RECEIVE_LED 0x01
#define GREEN_LED   0x02

/* Function prototypes */
void sleep(unsigned int count);

typedef struct msp430_impl{
 	int ID;
 	char *message;
 	int chat_IDs[4];
 	msp430_obj *signal_next;
 	int state;
 }msp430_obj; 




extern msp430_obj *root;

//Initialization for receiving messages
void receive_message(void){
	
	/* Filter setup return value: success or failure */
	unsigned char status;
	
	/* Set a filter address for packets received by the radio
	 *   This should match the "destination" address of
	 *   the packets sent by the transmitter. */
	uint8_t address[] = {0x12,0x34,0xab,0xcd};
	
	
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
	//MRFI_GpioIsr();
	
	/* Main loop just toggles the green LED with some delay */
	__bis_SR_register(GIE);
	/*while(1){
		__enable_interrupt();
		sleep(60000);
		P1OUT ^= GREEN_LED;
	}*/
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
		char packet_message[20];
		//packet_message = (char *) malloc(2);
		uart_puts("\nRadio Message Received!\n");
		root->message = "empty";
		uart_puts("sender: ");
		sprintf(packet_message,"%d",sender_id);
		uart_puts(packet_message);
		root->message = "empty2";
		uart_puts("\nmessage: ");
		sprintf(packet_message,"%s", &packet.frame[11]);
		uart_puts(packet_message);
		root->message = "\0";
		uart_putc('\n');
		free(packet_message);
		
		if(root->state == NETWORK_MODE || root->state == CHAT_ACCEPT_MODE){
			msp430_obj *senders = (msp430_obj*)malloc(sizeof(msp430_obj));
			msp430_obj *new_msp430;
			senders = root;
			while(senders != NULL)
			{
				//Only add to linked list if it's a new msp430
				if(senders->signal_next == NULL){
					new_msp430 = (msp430_obj*) malloc(sizeof(msp430_obj));
					
					new_msp430->ID = sender_id;
					new_msp430->chat_IDs[0]=sender_want_chat_id;
					new_msp430->state = NETWORK_MODE;
					new_msp430->signal_next = root->signal_next;
					
					root->signal_next = new_msp430;
					break;
				}
				else if(senders->ID == sender_id){
					break;
				}
				senders = senders->signal_next;
			}
			free(senders);
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
		uart_puts("DEBUG: Message Blocked\n");
		
	}
	
	
	
	/* We ignore the data in the packet structure.
	 * You probably want to do something with it here. */
	 
	/* Toggle the red GREEN to signal that data has arrived */
	
	P1OUT ^= GREEN_LED;
	sleep(60000);
	P1OUT ^= GREEN_LED;
}


