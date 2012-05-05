/*------------------------------------------------------------
 * Example radio "send" application
 *   Sends packets with a data payload 
 *----------------------------------------------------------*/
#include "3140_finalproject.h"

void send_message (char *msg){

		/* Construct a packet to send over the radio.
		 * 
		 *  Packet frame structure:
		 *  ---------------------------------------------------
		 *  | Length (1B) | Dest (4B) | Source (4B) | Payload |
		 *  ---------------------------------------------------
		 * Payload is:
		 * 1B for sender ID
		 * 1B for chat_ID
		 * The rest is variable for the wanted message
		 */
		mrfiPacket_t 	packet;
		
		//clear packet
		memset(&packet.frame[0], 0, sizeof(packet.frame));
		
		/* First byte of packet frame holds message length in bytes */
		packet.frame[0] = strlen(msg) + 14;	/* Includes 8-byte address header */
		
		/* Next 8 bytes are addresses, 4 each for source and dest. */
		packet.frame[1] = 0x12;		/* Destination */
		packet.frame[2] = 0x34;
		packet.frame[3] = 0xac;
		packet.frame[4] = 0xcd;
		
		packet.frame[5] = 0x02;		/* Source */
		packet.frame[6] = 0x00;
		packet.frame[7] = 0x01;
		packet.frame[8] = 0x02;
		
		//Frame 9 for input ID, frame 10 for wanted ID
		packet.frame[9] = root->ID;
		packet.frame[10] = root->chat_IDs[0];
		packet.frame[11] = root->chat_IDs[1];
		packet.frame[12] = root->chat_IDs[2];
		packet.frame[13] = root->chat_IDs[3];
		packet.frame[14] = root->chat_IDs[4];
		
		
		/* Remaining bytes are the message/data payload */
		strcpy( (char *) &packet.frame[15] , msg );
		
		/* Toggle red LED before transmitting, then wait a while */
		P1OUT ^= RED_SEND_LED;
		
		/* Transmit the packet over the radio */
		MRFI_Transmit(&packet , MRFI_TX_TYPE_FORCED);
		
		sleep(100000);
		P1OUT ^= RED_SEND_LED;
	
}
