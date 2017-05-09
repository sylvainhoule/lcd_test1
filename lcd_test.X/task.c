#include "include/task.h"

//global and broadcast
extern int sec,amp,freq;
extern int wav;
//freq=1000, wav=0;
extern char textBuff1[16], textBuff2[16];
extern unsigned char sinus[RESOL], sawtooth[RESOL], square[RESOL];

//global semaphore
char disp_f1=1;
char disp_f2=1;

long TickGet_ms(void){

	return TickGet()/(TIMER_FREQ/1000);


}

long  TickGet_sec(void){

	return TickGet()/(TIMER_FREQ);


}

void dispBufUpdate2(int _freq, int _amp, int _wav, int _sec){
    unsigned int percent=0;
    
            //percent=(unsigned int)_amp*100/AMPL;
            percent=(unsigned int)_amp*100/RESOL;
            PR2=((long)FCY/_freq)/RESOL;
            sprintf(textBuff1,"%dHz     %d    ",_freq,percent);

            if(_wav==0) sprintf(textBuff2,"     squa %d  ", _sec);
            else if(_wav==1)sprintf(textBuff2,"     sawt %d  ", _sec);
            else sprintf(textBuff2,"     sine %d  ", _sec); 
}


	
void initWave(int _amp){
		int i=0;
		for(i=0;i<RESOL;i++){//square LUT
			if(i>=RESOL/2)	square[i]=_amp;
			else square[i]=0;
		}

		for(i=0;i<RESOL;i++){
			sinus[i]=(_amp/2)*sin(TWO_PI*i/RESOL)+(_amp/2);//Sinus wave formula
			sawtooth[i]=((long)i*_amp)/RESOL;//Sawtooth wave
		}
	
	}

void decodeTask(void) {

  static DWORD lastTick;
  static int curPBs=0;
  static enum { SM_PBPoll =0, SM_Deb, SM_PBDecod,SM_FREQ, SM_WAV, SM_AMP, SM_PBRelease }state= 0;

	switch(state){
		case SM_PBPoll:
          // to convert into a function int readPBs(void)
		  //curPBs=readPBs();
          curPBs=(_RD6<<3) | (_RD7<<2) | (_RA7<<1)| (_RD13);
          
           // curPBs=PORTD
            if (curPBs != 0x000f){  //if (curPBs == 0x000fui)
                state= SM_Deb;
                lastTick= TickGet_ms(); 
            }
			break;
		case SM_Deb:
			if(TickGet_ms() - lastTick >=20){
				state=SM_PBDecod;
			}
			break;
        case SM_PBDecod:
			switch(curPBs){
                //Amp Up
                case 0b0101:
                    state=SM_PBRelease;
                    if (amp<RESOL)amp++;
                    initWave(amp);
                    //dispBufUpdate(0,0);
                    disp_f1=1;
                    break;
                //Amp Dn
                case 0b0110:
                    state=SM_PBRelease;
                    if (amp>0)amp--;
                    initWave(amp);
                  //  dispBufUpdate(0,0);
                    disp_f1=1;
                    break;
                //Freq Up
                case 0b1001:
                    state=SM_PBRelease;
                    if(freq<100000)freq+=INC_STEP;
                   // dispBufUpdate(0,0);
                    disp_f1=1;
                    break;
                //Freq Dn
                case 0b1010: 
                    state=SM_PBRelease;
                    if(freq>5)freq-=50;
                  //  dispBufUpdate(0,0);
                    disp_f1=1;
                    break;
                //change wave form Up or Dwn
                case 0b1101: 
				//case 0b1110: //dwn TODO
                    state=SM_PBRelease;
                    if(++wav>2)wav=0;
                  //  dispBufUpdate(1,wav);
                    disp_f1=1;
                    break;
                default: state=SM_PBPoll;
            }
            dispBufUpdate2(freq,amp,wav,sec);
			break;
        case SM_FREQ:
			
			break;
        case SM_WAV:
			
			break;
        case SM_AMP:
			
			break;
        case SM_PBRelease:
            curPBs=(_RD6<<3) | (_RD7<<2) | (_RA7<<1)| (_RD13);
			if((curPBs&0x0003)==0x0003)state=SM_PBPoll;
			break;


	}

}

void decodeRepeatTask(void) {

  static DWORD  lastTick;
  static DWORD  delay =1000;             // for auto repeat initial delay
  static int    cnt=0;                   // for auto repeat count
  static char curPBs=0, memPBs=0;
  static enum { SM_PBPoll =0, SM_Deb, SM_PBDecod,SM_FREQ, SM_WAV, SM_AMP, SM_PBRelease }state= 0;

	switch(state){
		case SM_PBPoll:
          // to convert into a function int readPBs(void)
		  //curPBs=readPBs();
          curPBs=((_RD6<<3) | (_RD7<<2) | (_RA7<<1)| (_RD13)) & 0x0F;
          memPBs=curPBs;  //memorize current PB state
           // A PB was pressed
            if (curPBs != 0x0f){  //if (curPBs == 0x000fui)
                state= SM_Deb;
                lastTick= TickGet_ms(); 
            }
			break;
		case SM_Deb:
			if(TickGet_ms() - lastTick >=20){
                curPBs=((_RD6<<3) | (_RD7<<2) | (_RA7<<1)| (_RD13)) & 0x0F;
                if(curPBs == memPBs) state=SM_PBDecod;
                else state = SM_PBPoll;
                delay =1000;           
                cnt=0;             
			}
			break;
        case SM_PBDecod:
            lastTick= TickGet_ms();
			switch(curPBs){
                //Amp Up
                case 0b0101:
                    state=SM_PBRelease;
                    if (amp<RESOL)amp++;
                    initWave(amp);
                    //dispBufUpdate(0,0);
                    disp_f1=1;
                    break;
                //Amp Dn
                case 0b0110:
                    state=SM_PBRelease;
                    if (amp>0)amp--;
                    initWave(amp);
                  //  dispBufUpdate(0,0);
                    disp_f1=1;
                    break;
                //Freq Up
                case 0b1001:
                    state=SM_PBRelease;
                    if(freq<100000)freq+=INC_STEP;
                   // dispBufUpdate(0,0);
                    disp_f1=1;
                    break;
                //Freq Dn
                case 0b1010: 
                    state=SM_PBRelease;
                    if(freq>5)freq-=50;
                  //  dispBufUpdate(0,0);
                    disp_f1=1;
                    break;
                //change wave form Up or Dwn
                case 0b1101: 
				//case 0b1110: //dwn TODO
                    state=SM_PBRelease;
                    if(++wav>2)wav=0;
                  //  dispBufUpdate(1,wav);
                    disp_f1=1;
                    break;
                default: state=SM_PBPoll;
            }
            dispBufUpdate2(freq,amp,wav,sec);
			break;
        case SM_FREQ:
			
			break;
        case SM_WAV:
			
			break;
        case SM_AMP:
			
			break;
        case SM_PBRelease:
            curPBs=(_RD6<<3) | (_RD7<<2) | (_RA7<<1)| (_RD13);
            if(memPBs != curPBs) state=SM_PBPoll;
            else if(TickGet_ms() - lastTick >=delay){
                state=SM_PBDecod;
                if (cnt++ > 4) delay =200; // decrease delay to 200 mS
            }
			break;


	}

}


void countSecTask(void) {

  static DWORD lastTick;
  static enum { SM_IDLE =0, SM_INC}state= 0;

	switch(state){
		case SM_IDLE:
			if(TickGet_ms() - lastTick >=1000){
				state=SM_INC;   	// memorize the current time
			}
			break;


		case SM_INC:
            lastTick= TickGet_ms();   	// memorize the current time
            sec++;
            dispBufUpdate2(freq,amp,wav,sec);
            disp_f1=1;   //set semaphore
            _LATA0 ^=1;
			state = SM_IDLE;
			break;
	}

}

//non-blocking loop
void displayTask(void) {
//    static DWORD lastTick;
    static char i=0;
    static char debug=0;
   static enum {SM_IDLE,SM_HOME1,SM_HOME2, SM_DISPLAY1, SM_DISPLAY2} state=0;
   debug++;
   switch (state) {
      	case SM_IDLE:
            if(disp_f1){
                state=SM_HOME1;
                disp_f1=0;   //reset semaphore
            }
         /*   if(disp_f2){
                state=SM_HOME2;
                disp_f2=0;   //reset semaphore
            }
           */ 
         break;
         
      	case SM_HOME1:
           // _LATG12=1;
            LCDL1Home();    //1200uS
            i=0;
            state=SM_DISPLAY1;
          //  _LATG12=0;
         break;
      	
      	case SM_DISPLAY1:
            
            if(textBuff1[i] !=0){
             // _LATG12=1;
             _LATG15=1;
                LCDPut(textBuff1[i]);//64uS
                i++;
             // _LATG12=0;
              _LATG15=0;
            }
            else state = SM_HOME2;
            break;
            
        case SM_HOME2:
            LCDL2Home();
            i=0;
            state=SM_DISPLAY2;
            break; 
        
       case SM_DISPLAY2:
             //non-blocking loop
            if(textBuff2[i] !=0){
                LCDPut(textBuff2[i]);
                i++;
            }
            else state = SM_IDLE;
         break;
   }
}

//Blocking loop
void displayTask2(void) {
    //static DWORD lastTick;
    static char i=0;
    static char debug=0;
   static enum {SM_IDLE,SM_HOME1,SM_HOME2, SM_DISPLAY1, SM_DISPLAY2} state=0;
   debug++;
   switch (state) {
      	case SM_IDLE:
            if(disp_f1){
                state=SM_HOME1;
                disp_f1=0;   //reset semaphore
            }      
         break;
         
      	case SM_HOME1:
            LCDL1Home();
            i=0;
            state=SM_DISPLAY1;
         break;
      	
      	case SM_DISPLAY1:
          //  _LATG13=1;
            //Blocking loop
            while(textBuff1[i] !=0){
                LCDPut(textBuff1[i]);
                i++;
            }
            //_LATG13=0;
            state = SM_HOME2;
         break;
         case SM_HOME2:
            LCDL2Home();
            i=0;
            state=SM_DISPLAY2;
            break;
         
         case SM_DISPLAY2:
            while(textBuff2[i] !=0){
                LCDPut(textBuff2[i]);
                i++;
            }
            state = SM_IDLE;
         break;
   }
}

/*
void dispBufUpdate(char option, int val){
    unsigned int percent=0;
    switch(option){
        case 0:
            percent=(unsigned int)amp*100/RESOL;
            PR2=((long)FCY/freq)/RESOL;
            sprintf(textBuff1,"%dHz     %d    ",freq,percent);
            break;
        case 1:
            if(val==0) sprintf(textBuff2,"     square    ");
            else if(val==1)sprintf(textBuff2,"     sawtooth    ");
            else sprintf(textBuff2,"     sine    ");
            break;
   
    }
}
*/