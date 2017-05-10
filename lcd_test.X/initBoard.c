#include "include/initBoard.h"
#include "include/task.h"
//BRANCH

// CONFIG2
#pragma config POSCMOD = NONE    // Primary Oscillator Select->Primary oscillator disabled
#pragma config OSCIOFNC = OFF    // Primary Oscillator Output Function->OSC2/CLKO/RC15 functions as CLKO (FOSC/2)
#pragma config FCKSM = CSDCMD    // Clock Switching and Monitor->Clock switching and Fail-Safe Clock Monitor are disabled
#pragma config FNOSC = FRCPLL    // Oscillator Select->Fast RC Oscillator with PLL module (FRCPLL)
#pragma config IESO = ON    // Internal External Switch Over Mode->IESO mode (Two-Speed Start-up) enabled

// CONFIG1
#pragma config WDTPS = PS32768    // Watchdog Timer Postscaler->1:32,768
#pragma config FWPSA = PR128    // WDT Prescaler->Prescaler ratio of 1:128
#pragma config WINDIS = ON    // Watchdog Timer Window->Standard Watchdog Timer enabled,(Windowed-mode is disabled)
#pragma config FWDTEN = ON    // Watchdog Timer Enable->Watchdog Timer is enabled
#pragma config ICS = PGx2    // Comm Channel Select->Emulator/debugger uses EMUC2/EMUD2
#pragma config COE = OFF    // Set Clip On Emulation Mode->Reset Into Operational Mode
#pragma config BKBUG = OFF    // Background Debug->Device resets into Operational mode
#pragma config GWRP = OFF    // General Code Segment Write Protect->Writes to program memory are allowed
#pragma config GCP = OFF    // General Code Segment Code Protect->Code protection is disabled
#pragma config JTAGEN = ON    // JTAG Port Enable->JTAG port is enabled

void OSCILLATOR_Initialize(void)
{
    // NOSC FRCPLL; SOSCEN disabled; OSWEN Switch is Complete; 
    __builtin_write_OSCCONL((uint8_t) (0x0100 & 0x00FF));
    // RCDIV FRC/1; DOZE 1:8; DOZEN disabled; ROI disabled; 
    CLKDIV = 0x3000;
    // TUN Center frequency; 
    OSCTUN = 0x0000;
}

/*
	Function that set timer 3 to...
	input: 
	output:

*/
void initTimer3( void)
{
    // init the timebase
    T3CON = 0x8000;         // enable TMR3, prescale 1:1, internal clock
    PR3 = RESOL-1;            // set the period for the given bitrate: PWM frequency = fcy/PR3 (e.g. 16MHz/64=250kHz)
    _T3IF = 0;              // clear interrupt flag
  //  _T3IE = 1;              // enable TMR3 interrupt

} 


void initTimer2( void)
{
// Initialize and enable Timer2
	T2CONbits.TON = 0; // Disable Timer
	T2CONbits.TCS = 0; // Select internal instruction cycle clock
	T2CONbits.TGATE = 0; // Disable Gated Timer mode
	T2CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
	TMR2 = 0x00; // Clear timer register
	PR2=RESOL-1; // Load the period value. It specifies waveform frequency: (fcy/(PR2+1))/RESOL (e.g. 16MHZ/64/64= 3.9kHz)
	_T2IP = 0x01; // Set Timer 2 Interrupt Priority Level
	_T2IF = 0; // Clear Timer 2 Interrupt Flag
	IEC0bits.T2IE = 1; // Enable Timer 2 interrupt
    _T2IP = 6; // Enable Timer 2 interrupt
	T2CONbits.TON = 1; // Start Timer             

} // initAudio

void initIO(void){
    _TRISA0=0;
    _TRISA1=0;
    _TRISD6=1;
    _TRISD13=1;
    _TRISA7=1;
    _TRISD7=1;
    _TRISG13=0;
    _TRISG12=0;
       _TRISG15=0;
 
}
void initOC( void)
{
    // init PWM
    // set the initial duty cycles
    OC1R = OC1RS = 25;  
    // activate the PWM modules 
    OC1CON = 0x000E;        // CH1 and CH2 in PWM mode, TMR3 based

} 
