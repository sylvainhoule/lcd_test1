/* UART functions header file COMM2.h
 *		
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Serge Hould	March 2010	First version of source file, tested on the target  - v1
* Serge Hould	December 2016	Add header										- v1.1
*								Add with function without flow control
*								Add an ISR 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef COMM2_H
#define COMM2_H
void initUart2( void);
void initUart2_wInt( void);
int putc2( char c);
int putc2_noHard(char c);
char getc2( void);
void puts2( char *str );
void outUint8(unsigned char u8_x);
void putI8(unsigned char u8_x);

#endif //COMM2_H

