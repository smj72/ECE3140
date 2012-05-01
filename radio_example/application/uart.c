#include "3140_finalproject.h"

volatile int index = 0;
char out[255];
char another[255];
msp430_obj my_msp430;

typedef struct msp430_impl{
 	int ID;
 	char message [255];
 	msp430_obj *signal_next;
 	
 }msp430_obj;

/*------------------------------------------------------------------------
 * UART communication functions
 *----------------------------------------------------------------------*/

/* Initialize the UART for TX (9600, 8N1) */
/* Settings taken from TI UART demo */ 
void init_uart(msp430_obj *msp430_ptr) {
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
	
	__bis_SR_register(LPM3_bits + GIE);       /* Enter LPM3, interrupts enabled */
	
	set_msp430(*msp430_ptr);				//Initialize msp430 to this uart.
}

void set_msp430(msp430_obj msp430)
{
	my_msp430 = msp430;
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
	uart_putc(UCA0RXBUF);
	
	int i =0;
	out[index++] = UCA0RXBUF;
	//clears out the rest after end
	if (UCA0RXBUF == '\r'){
		uart_puts("\r\nEntered:\n");
		//i =0;
		for(int i=0;i<255;i++){
			if (i<index){
				another[i] = out[i];
			}else{
				another[i] = 0;
				out[i] = 0;
			}
		}
		index = 0;
		my_msp430.message = another;
		uart_puts(&another[0]);
		uart_putc('\n');
	}else if(index == 255){
		uart_puts("\r\nThe limit is 255 characters, your entry has been restarted.\r\n");
		//i = 0;
		for(int i = 0;i<255;i++) {out[i]=0;}
		index = 0;
	}
	else{
		uart_puts("Proper radio message not received");
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

 /*Function to execute upon receiving a packet
 *   Called by the driver when new packet arrives */
 ///*
void MRFI_RxCompleteISR(void) {
/*   Since this is a transmit-only application we do nothing.
 *   This function still needs to be defined for the project
 *   to compile properly. 
 * */
}
///*
int main (void) {
	WDTCTL = WDTPW + WDTHOLD;
 	P1DIR = 0x03;
 	P1OUT = 0x00;
 	init_uart(NULL);	
 	uart_clear_screen();
 	
 	while (1) ;
 	return 0;
}
//*/