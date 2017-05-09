/**
  
*/

#ifndef TASK_H
#define TASK_H
/**
    Section: Includes
*/
#include "..\..\common\GenericTypeDefs.h"
#include "..\..\common\tickFast.h"
#include "..\..\common\pmp_lcd.h"
#include <stdint.h>
#include <stdio.h>
#include <xc.h>
#include <math.h>

/**
    Section: Macros
*/
//macros definitions

#define RESOL	64
#define FCY     16000000ull
#define TIMER_FREQ	FCY/8   //2000000
#define LED7 _LATA7
#define LED6 _LATA7
#define PB  _RD13
#define A_SET   _RD6
#define F_SET   _RD7
#define UP      _RA7
#define DN     _RD13
#define AMPL     40
#define FREQ    1
#define TWO_PI 		6.2831
#define INC_STEP    50


/**
    Section: Function Prototypes
*/
void decodeTask(void);
void decodeRepeatTask(void);
void countSecTask(void);
void displayTask(void);
void displayTask2(void);
long TickGet_ms(void);
long  TickGet_sec(void);
void initWave(int);
void dispBufUpdate2(int , int , int , int );
#endif
