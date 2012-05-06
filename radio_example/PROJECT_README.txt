ECE 3140 Final Project: Chat system
Edgar Munoz		em447
Sanford Johnson		smj72
===================================================

This MSP430 Project utilizes the radio system in order to 
provide a chat interface between 2 MSP430s.

Program Requirements
------------------------------
1. A terminal program such as Termite
   Termite can be located at http://www.compuphase.com/software_termite.htm
   Settings: Baud rate = 9600
             set the port to your MSP430 (COM 3 or 4 typically)
             Set append CR
             Leave everything else to the default: 
             (data bits 8 stop bits 1 parity and flow control none)



Program Instructions
------------------------------

 1. Import the project in Code Composer Studio and compile/run the code.

 2. Make sure your terminal is synced with your MSP430.

 3. In the beginning, one is required to set an ID as a uart input

 4. Put in another ID (Use /find to retrieve a list of available IDs)
    to send a chat request

 5. Respond to the request by writing the same ID back. Chat mode begins.

 6. Once in chat mode, write messages to the other msp430. There are times where
    signals are not always retrieved properly, so make sure that it was sent properly.


Uart Commands
-------------------------

1. /quit: quits out of chat. If not in chat, allows one to choose another ID.

2. /find: Search for other MSP430s in the area and retrieve their IDs.
