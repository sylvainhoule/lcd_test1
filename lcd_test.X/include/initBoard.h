/**
  
*/

#ifndef INITBOARD_H
#define INITBOARD_H
/**
    Section: Includes
*/
#include <stdint.h>
#include <xc.h>
/**
    Section: Macros
*/
//macros definitions
/**
    Section: Function Prototypes
*/
void OSCILLATOR_Initialize(void);
void initTimer3( void);
void initTimer2( void);
void initIO(void);
void initOC( void);

#endif
