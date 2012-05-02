/*------------------------------------------------------------
 * Example radio "send" application
 *   Sends packets with a data payload in an infinite loop
 *----------------------------------------------------------*/

#include "bsp.h"
#include "mrfi.h"
#include "radios/family1/mrfi_spi.h"
#include "3140_finalproject.h"
#include <string.h>

/* Useful #defines */
#define RED_SEND_LED 		0x01

/* Function prototypes */
//void sleep(unsigned int count);



typedef struct msp430_impl{
 	int ID;
 	char *message;
 	int chat_IDs[20];
 	msp430_obj *signal_next;
 	int state;
 }msp430_obj; 

extern msp430_obj *root;



void send_message (void){
	/* Set red LED to output */
	//P1DIR = RED_SEND_LED;
	//P1OUT ^= RED_SEND_LED;
	
	
	/* Main (infinite) transmit loop */
	//while(1){
		int i = 0;
		/* Construct a packet to send over the radio.
		 * 
		 *  Packet frame structure:
		 *  ---------------------------------------------------
		 *  | Length (1B) | Dest (4B) | Source (4B) | Payload |
		 *  ---------------------------------------------------
		 */
		mrfiPacket_t 	packet;
		
		//clear packet
		for(i;i<sizeof(packet.frame);i++){
			packet.frame[i]=0;
		}
		
		//char msg[] = "ECE3140 rocks!\r\n"; 
		//char msg[] = *msp430.message;
		/* First byte of packet frame holds message length in bytes */
		packet.frame[0] = strlen(root->message) + 8;	/* Includes 8-byte address header */
		
		/* Next 8 bytes are addresses, 4 each for source and dest. */
		packet.frame[1] = 0x12;		/* Destination */
		packet.frame[2] = 0x34;
		packet.frame[3] = 0xab;
		packet.frame[4] = 0xcd;
		
		packet.frame[5] = 0x02;		/* Source */
		packet.frame[6] = 0x00;
		packet.frame[7] = 0x01;
		packet.frame[8] = 0x02;
		
		//Frame 9 for input ID, frame 10 for wanted ID
		packet.frame[9] = root->ID
		
		packet.frame[10] = root->chat_IDs[0];
		
		
		/* Remaining bytes are the message/data payload */
		strcpy( (char *) &packet.frame[11] , root->message );
		
		
		/* Transmit the packet over the radio */
		MRFI_Transmit(&packet , MRFI_TX_TYPE_FORCED);
		
		/* Toggle red LED after transmitting, then wait a while */
		P1OUT ^= RED_SEND_LED;
		sleep(60000);
		P1OUT ^= RED_SEND_LED;
	//}
}




