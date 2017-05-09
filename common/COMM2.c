/********************************************************************************************
*  	File Name:     COMM2.c
*	
*	Name: Serial library.
* 
*	Description: Uses UART2 RS232 for Pic24 explorer16.
*      
* * REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author        	Date      	Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Serge Hould	March 2010	First version of source file, tested on the target  - v1
* Serge Hould	December 2016	Add header										- v1.1
*								Add with function without flow control
*								Add an ISR 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	TODO:
		
*********************************************************************************************/


#include "COMM2.h"
#include <xc.h>

//
// I/O definitions for the Explorer16 using hardware flow control
#define CTS    	_RF12              // Cleart To Send, input, HW handshake
#define RTS     _RF13               // Request To Send, output, HW handshake
#define TRTS    TRISFbits.TRISF13   // tris control for RTS pin

/* U2BRG (BRATE)
U2BRG = (Fosc / 8 / baudrate) -1 ; for BREGH=1
In our case this translates to the following expression:
 */

/* U2BRG (BRATE)
U2BRG = (Fosc / 32 / baudrate) -1 ; for BREGH=0
In our case this translates to the following expression:

*/
// timing and baud rate calculations
//#define BRATE   33        // 115200 baud (BREGH=1) for PIC24  at 32MHz  BRATE=33 
#define BRATE   	417        // 9600 baud (BREGH=1) for PIC24  at 32MHz  BRATE=417 
#define U_ENABLE 	0x8008      // enable the UART peripheral (BREGH=1)
#define U_TX    	0x0400      // enable transmission
   
/**********************************
 Initialize the UART2 serial port
**********************************/
void initUart2( void)
{
   U2BRG    = BRATE;    
   U2MODE    = U_ENABLE;
   U2STA    = U_TX;
   TRTS    = 0;        // make RTS output
   RTS     = 1;        // set RTS default status
} // initUart
/**********************************
 initialize the UART2 serial port 
 with interrupt.  
 See ISR at the end of this document
 **********************************/
void initUart2_wInt( void)
{
    /* Calcul de U2BRG (BRATE)*/
    //U2BRG = (Fcy/ 4 / baudrate) -1 ; for BREGH=0
   U2BRG    = BRATE ;    // 16M/4/9600 = 417
   U2MODE    = U_ENABLE ;     // enable the UART peripheral (BREGH=1)
   U2STA    = U_TX;      // enable transmission
   _U2RXIF=0;  
   _U2RXIP=1;  // Interrutp priority 1
   _U2RXIE=1;  // if interrupt driven RX only

} // initUart
/****************************************
Send a singe character to the UART2 
serial port.

input: 
	Parameters:
		char c 	character to be sent
output:
	return:
		int		return the character sent.
*****************************************/
int putc2(char c)
{
   while ( CTS);              // wait for !CTS, clear to send
   while ( U2STAbits.UTXBF);   // wait while Tx buffer full
   U2TXREG = c;
   return c;
} 
/****************************************
Same as putc2() but  w/o hardware control
*****************************************/
int putc2_noHard(char c)
{
   while ( U2STAbits.UTXBF);   // wait while Tx buffer is still full
   U2TXREG = c;
   return c;
}
/****************************************
*****************************************/
// wait for a new character to arrive to the UART2 serial port
char getc2( void)
{
    RTS = 0;            // assert Request To Send !RTS
   while ( !U2STAbits.URXDA);   // wait for a new character to arrive
   return U2RXREG;      // read the character from the receive buffer
   RTS = 1;
}// 


   /*******************************************************************************
   Function: puts2( char *str )

   Precondition:
      initUart must be called prior to calling this routine.

   Overview:
      This function prints a string of characters to the UART.

   Input: Pointer to a null terminated character string.

   Output: None.

   *******************************************************************************/
   void puts2( char *str )
   {
      unsigned char c;

      while( (c = *str++) )
         putc2(c);
   }

   
   // to erase because putI8 supercedes it
void outUint8(unsigned char u8_x) {
  unsigned char u8_c;
   putc2('0');
   putc2('X');
  u8_c = (u8_x>>4)& 0xf;
  if (u8_c > 9) putc2('A'+u8_c-10);
  else putc2('0'+u8_c);
  //LSDigit
  u8_c= u8_x & 0xf;
  if (u8_c > 9) putc2('A'+u8_c-10);
  else putc2('0'+u8_c);
}
/****************************************
Precondition:
    initUart2() must be called prior to calling 
	this routine.

	Overview: send an 8 bit integer value to 	
	the UART2 serial port
	Example: putI8(55);
*****************************************/
void putI8(unsigned char u8_x) {
  unsigned char u8_c;
   putc2('0');
   putc2('X');
  u8_c = (u8_x>>4)& 0xf;
  if (u8_c > 9) putc2('A'+u8_c-10);
  else putc2('0'+u8_c);
  //LSDigit
  u8_c= u8_x & 0xf;
  if (u8_c > 9) putc2('A'+u8_c-10);
  else putc2('0'+u8_c);
}
/****************************************
	ISR for Uart 2 rx
*****************************************/
/*
 void _ISR_NO_PSV _U2RXInterrupt(void)
{
            _U2RXIF=0;

            static char i=0;
            static int recBuffer[50];

            recBuffer[i++] = U2RXREG; // received data

}*/