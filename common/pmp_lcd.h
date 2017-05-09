/**********************************************************************
*	Serge Hould
* 	
*		REVISION HISTORY: 
*
*			Date: 19 Aug. 2016 Delay modified for the profiling part of the function Generator lab 
*                  using Explorer 16 at fcy = 16MHz. Two delays possible by defining SLOW or FAST.
*					See comments.
* 					
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#ifndef __PMP_LCD_H_
#define __PMP_LCD_H_

#include "p24Fxxxx.h"
#include "pmp.h"
//#include "../common/pmp.h"

#define 	SLOW	// for slow LCD
//#define 	FAST	// for fast LCD

#ifdef		SLOW		// for slow LCD, delays modified for the profiling part of the function Generator 
						// lab using Explorer 16 at fcy = 16MHz
// Define a fast instruction execution time in terms of loop time
// typically > 43us
#define	LCD_F_INSTR		1700 //1.04 mS measured, Explorer 16 at fcy = 16MHz

// Define a slow instruction execution time in terms of loop time
// typically > 1.35ms
#define	LCD_S_INSTR		2000//1.22mS measured, Explorer 16 at fcy = 16MHz

// Define the startup time for the LCD in terms of loop time
// typically > 30ms
#define	LCD_STARTUP		1400000   //14000  
#endif

#ifdef		FAST		// for fast LCD, delays optimized for Explorer 16 at fcy 16MHz

// Define a fast instruction execution time in terms of loop time
// typically > 43us
#define	LCD_F_INSTR		70  //42uS measured, Explorer 16 at fcy = 16MHz

// Define a slow instruction execution time in terms of loop time
// typically > 1.35ms
#define	LCD_S_INSTR		150//76 uS measured, Explorer 16 at fcy = 16MHz    

// Define the startup time for the LCD in terms of loop time
// typically > 30ms
#define	LCD_STARTUP		2000
#endif

void LCDPos2(unsigned char row);
void LCDPos1(unsigned char row);
void LCDInit(void);
void LCDHome(void);
void LCDL1Home(void);
void LCDL2Home(void);
void LCDClear(void);
void LCDPut(char A);
void Wait(unsigned int B);
void DisplayMSG (char *);
void pmp_Init(void);
void LCDPutString( char *array);



#endif
