#include "3140_finalproject.h"

volatile int index = 0;
char out[CHAR_LIMIT];

msp430_obj *root;

/*------------------------------------------------------------------------
 * UART communication functions
 *----------------------------------------------------------------------*/

/* Initialize the UART for TX (9600, 8N1) */
/* Settings taken from TI UART demo */ 
void init_uart(void) {
	msp430_obj *root_init = (msp430_obj*)malloc(sizeof(msp430_obj));
	char *msg_init = (char*)malloc(CHAR_LIMIT);
	BCSCTL1 = CALBC1_1MHZ;        /* Set DCO for 1 MHz */
	DCOCTL  = CALDCO_1MHZ;
	P3SEL = 0x30;                 /* P3.4,5 = USCI_A0 TXD/RXD */
	UCA0CTL1 |= UCSSEL_2;         /* SMCLK */
	UCA0BR0 = 104;                /* 1MHz 9600 */
	UCA0BR1 = 0;                  /* 1MHz 9600 */
	UCA0MCTL = UCBRS0;            /* Modulation UCBRSx = 1 */
	UCA0CTL1 &= ~UCSWRST;         /* Initialize USCI state machine */
	IE2 |= UCA0RXIE; 			  /* Enable USCI_A0 RX interrupt */
	
	// Define Root parameters
	root_init->ID = 0;
	msg_init = "\0";
	root_init->message = msg_init;
	root_init->chat_want_ID = 0;
	root_init->signal_next = NULL;
	root_init->state = CHOOSE_ID_MODE;
	
	root = root_init;
	
	//__bis_SR_register(LPM3_bits + GIE);       /* Enter LPM3, interrupts enabled */
	__bis_SR_register( GIE);
}


/* Transmit a single character over UART interface */
void uart_putc(char c) {
    while(!(IFG2 & UCA0TXIFG)); /* Wait for TX buffer to empty */
    UCA0TXBUF = c;				/* Transmit character */
}

/* Transmit a nul-terminated string over UART interface */
void uart_puts(char *str) {
	while (*str) {
		/* Replace newlines with \r\n carriage return */
		if(*str == '\n') { uart_putc('\r'); }
		uart_putc(*str++);
	}
}

/* Clear terminal screen using VT100 commands */
/* http://braun-home.net/michael/info/misc/VT100_commands.htm */
void uart_clear_screen(void) {
	uart_putc(0x1B);		/* Escape character */
 	uart_puts("[2J");		/* Clear screen */
 	uart_putc(0x1B);		/* Escape character */
 	uart_puts("[0;0H");		/* Move cursor to 0,0 */
}

// USCI A0/B0 Receive ISR
// \r represents the end of a finished package send (I believe)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	char *pch;
	if(index == 0){
		//Clear buffer
		memset(&out[0], 0, sizeof(out));
	}

	// Grow a user input array with the characters received from user
	out[index++] = UCA0RXBUF;
	
	// If carriage return process the array constructed
	if (UCA0RXBUF == '\r'){
		
		root->message = out; 
		pch = strstr(root->message,"/");
		
		
		if (strncmp(pch,"/quit",index-1)==0){
			root->chat_want_ID = 0;
			root->signal_next = NULL;
			if(root->state == CHAT_MODE)
			{
				root->state = NETWORK_MODE;
				
				uart_puts("\nQuitting. Returning to chat select mode...\nPlease input the ID you wish to chat with.\n If you would like to choose another ID, please type in /quit again.\n");
			}
			else if(root->state == NETWORK_MODE){
				root->state = CHOOSE_ID_MODE;
				root->ID = 0;
				uart_puts("\nQutting. Returning to choose ID mode...\n Please select the ID you wish to use.\n");
			}
		}
		else if (strncmp(pch,"/find",index-1)==0) {
				send_message("/find");
				uart_puts("\nFinding other MSP430s...\n");
			}
		
		else{
			// Restart index to the beginning of the array
			
			uart_putc('\n');
			
			//State changes based on what was sent
			//Choose ID mode: obtain your ID
			if(root->state == CHOOSE_ID_MODE)
			{
				//int wanted_id = out[0] - '0';
				int wanted_id = atoi(out);
				root->ID = wanted_id;
				root->state = NETWORK_MODE;
				uart_puts("\n ID is now: ");
				uart_puts(out);
				uart_puts("\nNow choose another ID you wish to chat with\n");
			}
			//Network mode: send chat acceptance to msp430 with specific ID
			else if(root->state == NETWORK_MODE)
			{
				//int wanted_chat_id = out[0] - '0';
				int wanted_chat_id = atoi(out);
				if( root->chat_want_ID != wanted_chat_id && root->chat_want_ID!=0){
					send_message("/remove");
				}
				root->chat_want_ID = wanted_chat_id;
				uart_puts("\nSending chat request to ID ");
				uart_puts(out);
				uart_putc('\n');
			}
			//Chat Mode: send message to another msp430
			else if(root->state == CHAT_MODE){
				uart_puts("\nSending message:\n");
				uart_puts(root->message);
				uart_putc('\n');
				
				send_message(root->message);
			}
			index = 0;
		}
		
   // Overflow error, will only accept messages that are so many characters long
	}
	else if(index >= CHAR_LIMIT){
		uart_puts("\nThe limit is so many characters, Please try again.\n");
		//Clear buffer
		memset(&out[0], 0, sizeof(out));
		index = 0;
		//__bic_SR_register_on_exit(LPM3_bits);
	
	}
}

