#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"

unsigned int freqDetected=0;
char freqDetected_str[6];
unsigned int total_seconds=0;
int Nadc=0;

//-------------------------------------------------------------
//            state1 function
//-------------------------------------------------------------
void freqCounter(void){
	lcd_clear();     //clear LCD
	lcd_home();      //bring cursor to start of first line
	lcd_puts("fin = 00000 Hz"); 
	cursor_off;
	state1TIMERconfig();
	LPM0;         //enter LPM0 mode        
	while (state == state1){
		if (val2>val1){
			disable_interrupts();
			freqDetected = Fsmclk/(val2-val1);
			sprintf(freqDetected_str, "%05d", freqDetected); //add zeros to the left of the number to be 5 numbers
			//write freqDetected to lcd
			lcd_home();      //bring cursor to start of first line
			lcd_cursor_right(); 
			lcd_cursor_right();
			lcd_cursor_right();
			lcd_cursor_right();
			lcd_cursor_right();
			lcd_cursor_right();
			lcd_puts(freqDetected_str);
			cursor_off;	
			DelayMs(500);
			enable_interrupts();
		}
		LPM0;
	}
	lcd_clear();     //clear LCD
	haltTA1();
}
//--------------------------------------------------------------------
//            state2 function
//--------------------------------------------------------------------            
void stopWatch(void){
	lcd_clear();     //clear LCD
	lcd_home();      //bring cursor to start of first line
	//lcd_puts("00:00");
	state2TIMERconfig();

	while (state == state2){
		if (readSWs()==1){             //SW0='1' continue counting
			startTA0_st2();
			LPM0;         //enter LPM0 mode
			lcd_home();  										// M_left M_right : S_left S_right 
   			lcd_data(0x30 + ((total_seconds / 60) / 10));  		// M_left
   			lcd_data(0x30 + ((total_seconds / 60) % 10)); 		// M_right
   			lcd_data(':');										// :
   			lcd_data(0x30 + ((total_seconds % 60) / 10)); 		// S_left
   			lcd_data(0x30 + ((total_seconds % 60) % 10)); 		// S_right
   			cursor_off;
			total_seconds++;
			if(total_seconds==3600){
				total_seconds=0;
			}
		}
		else{                       //SW0='0' stop counting
			haltTA0();
			}
	}
	lcd_clear();     //clear LCD
}
//--------------------------------------------------------------------
//            state3 function
//--------------------------------------------------------------------            
void buzzer(void){
	state3TIMERconfig();
	state3ADCconfig();
	while(state==state3){
		sampleADC();            //start sample
		LPM0;         //enter LPM0 mode
		Nadc = readADC10MEM();
		SetFreqByADC(Nadc);
		
	}
	haltADC();
	haltTA1();
}


 
  

