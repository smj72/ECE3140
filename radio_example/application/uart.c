#include "3140_finalproject.h"

volatile int index = 0;
char out[255];
char another[255];

//msp430_obj my_msp430;
typedef struct msp430_impl{
 	int ID;
 	char *message;
 	int chat_IDs[20];
 	msp430_obj *signal_next;
 	int state;
 }msp430_obj; 
msp430_obj *root;

/*------------------------------------------------------------------------
 * UART communication functions
 *----------------------------------------------------------------------*/

/* Initialize the UART for TX (9600, 8N1) */
/* Settings taken from TI UART demo */ 
void init_uart(void) {
	msp430_obj *root_init = (msp430_obj*)malloc(sizeof(msp430_obj));
	char *msg_init = (char*)malloc(sizeof(char));
	int i = 0;
	BCSCTL1 = CALBC1_1MHZ;        /* Set DCO for 1 MHz */
	DCOCTL  = CALDCO_1MHZ;
	P3SEL = 0x30;                 /* P3.4,5 = USCI_A0 TXD/RXD */
	UCA0CTL1 |= UCSSEL_2;         /* SMCLK */
	UCA0BR0 = 104;                /* 1MHz 9600 */
	UCA0BR1 = 0;                  /* 1MHz 9600 */
	UCA0MCTL = UCBRS0;            /* Modulation UCBRSx = 1 */
	UCA0CTL1 &= ~UCSWRST;         /* Initialize USCI state machine */
	IE2 |= UCA0RXIE; 			  /* Enable USCI_A0 RX interrupt */
	//IE1  |=  URXIE0;        	  //  Enable  USART0  RX  interrupt
	//extern msp430_obj *root;
	
	// Define Root parameters
	root_init->ID = 5;
	msg_init = "\0";
	root_init->message = msg_init;
	for(i;i<sizeof(root->chat_IDs);i++){
	
		root_init->chat_IDs[i] = 0;
	}
	//msg_init = "/empty/";
	//root_init->message = NULL;
	root_init->signal_next = NULL;
	root_init->state = 0;
	
	root = root_init;
	
	//__enable_interrupt();
//	set_msp430(*msp430_ptr);				//Initialize msp430 to this uart.
	//__bis_SR_register(LPM3_bits + GIE);       /* Enter LPM3, interrupts enabled */
	__bis_SR_register( GIE);
}

/*void set_msp430(msp430_obj msp430)
{
	my_msp430 = msp430;
}*/

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
	int i =0;
	//if(index == 0){
		//Clear buffer
	//	memset(&out[0], 0, sizeof(out));
	//}

	// Grow a user input array with the characters received from user
	out[index++] = UCA0RXBUF;
	
	// If carriage return process the array constructed
	if (UCA0RXBUF == '\r'){
		//uart_puts("\r\nEntered:\n");
		i =0;
		for(i;i<255;i++){
			if (i<index){
				another[i] = out[i];
				root->message[i] = out[i];
			}else{
				another[i] = 0;
				out[i] = 0;
				root->message[i] = 0;
			}
		}
		
		root->message = another; 
		
		// Restart index to the beginning of the array
		index = 0;
		// Echo back the user input
		//uart_puts(&another[0]);
		uart_putc('\n');
		/*if(root->state == NETWORK_MODE){
			root->message = another;
		}*/
		//Clear buffer
		//memset(&out[0], 0, sizeof(out));
   // Overflow error, will only accept messages that are 255 characters long
	}else if(index == 255){
		uart_puts("\r\nThe limit is 255 characters, your entry has been restarted.\r\n");
		//Clear buffer
		//memset(&out[0], 0, sizeof(out));
		i = 0;
		// Clean buffer and restart at beginning of array
		for(i;i<255;i++) {out[i]=0;}
		index = 0;
	}
}

/*//Interrupt for receiving uart input
 #pragma  vector=USART0RX_VECTOR
  __interrupt  void  usart0_rx  (void)
  {
       while  (!(IFG1  &  UTXIFG0));        //  USART0  TX  buffer  ready?
       {
       	TXBUF0  =  RXBUF0; //  RXBUF0  to  TXBUF0
       }
       uart_puts(" message returned");
}*/


///*
/*int main (void) {
	msp430_obj *dummy = (msp430_obj*)malloc(sizeof(msp430_obj));
	char *dummy_msg = (char*)malloc(sizeof(char));
	WDTCTL = WDTPW + WDTHOLD;
 	P1DIR = 0x03;
 	P1OUT = 0x00;
 	
 	
 	dummy->ID = 1;
 	dummy->message = dummy_msg;
 	dummy->signal_next = NULL;
 	
 	init_uart(dummy);	
 	uart_clear_screen();
 	
 	while (1) ;
 	return 0;
}*/
//*/
