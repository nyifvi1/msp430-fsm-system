#include  "../header/bsp.h"    // private library - BSP layer

//-----------------------------------------------------------------------------  
//           GPIO congiguration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
 // volatile unsigned int i; // in case of while loop usage
  
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT
   
  // LEDs 8-bit Array Port configuration
  //LEDsArrPortSel &= ~0xFF;            // GPIO capability
  //LEDsArrPortDir |= 0xFF;             // output dir
  //LEDsArrPort = 0x00;				  // clear all LEDs
  
   // LCD Setup
  LCD_DATA_WRITE &= 0x0F;   //bic Px.7-Px.4
  LCD_DATA_DIR |= 0xF0;     //bis Px.7-Px.4
  LCD_DATA_SEL &= 0x0F;
  LCD_CNTL_SEL &= ~0xE0;    //bic Px.7-Px.5
  
  // Switches Setup
  SWsArrPortSel &= ~SW0;  //bic PxSel SW0      
  SWsArrPortDir &= ~SW0;  //bic PxDir SW0 
  
  // PushButtons Setup          PB-PB2 configuration
  PBsArrPortSel &= 0xF8; 
  PBsArrPortDir &= 0xF8;
  PBsArrIntEdgeSel |= 0x03;  	     // pull-up mode
  PBsArrIntEdgeSel &= ~0x04;         // pull-down mode
  PBsArrIntEn |= 0x07;
  PBsArrIntPend &= 0xF8;            // clear pending interrupts 

// PB3 Setup          //------------------------need to be edited//------------------------
  //P2SEL &= ~PB3;
  //P2DIR &= ~PB3;
  //P2IES &= ~PB3;                // pull-down mode
  //P2IE |= PB3;
  //P2IFG &= ~PB3;

  _BIS_SR(GIE);                     // enable interrupts globally
}                             
//------------------------------------------------------------------------------------- 
//            Timers congiguration 
//-------------------------------------------------------------------------------------
void TIMERconfig(void){
	P2DIR &= ~0x10;  //bic BIT4 generator connected to P2.4 Input Capture
	P2SEL |= 0x10;   //bis BIT4

        P2DIR |= 0x04;   //bis BIT2  P2.2 output comapre (PWM)
	P2SEL |= 0x04;   //bis BIT2
        P2SEL2 &= ~0x04; //bic BIT2
} 
//------------------------------------------------------------------------------------- 
//            ADC congiguration 
//-------------------------------------------------------------------------------------
void ADCconfig(void){
	ADC10AE0 |= BIT3;  //enable A3 as analog input
        ADC10CTL1 = INCH3;  // INCH3 - select A3 as input channel
        CAPD &= ~CAPD3;     //disable comparator port

} 

           
             

 
             
             
            
  

