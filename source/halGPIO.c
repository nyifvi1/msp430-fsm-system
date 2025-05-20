#include  "../header/halGPIO.h"     // private library - HAL layer

unsigned int val1=0;
unsigned int val2=0;
unsigned int n=33792;   //Q11.5 of 1056 (1kHz)
unsigned int m=20;	   //Q11.5 of 634/1023 (slope)
unsigned int res;
//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
	GPIOconfig();
	TIMERconfig();
	ADCconfig();
	lcd_init();
}
//--------------------------------------------------------------------
//				Read value of 4-bit SWs array 
//--------------------------------------------------------------------
unsigned char readSWs(void){
	unsigned char ch;
	
	ch = SWsArrPort;
	ch &= SWmask;     // mask the least 1-bit
	return ch;
}
//---------------------------------------------------------------------
//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  // t[msec]
	volatile unsigned int i;
	
	for(i=t; i>0; i--);
}
//---------------------------------------------------------------------
//            Enter from LPM0 mode
//---------------------------------------------------------------------
void enterLPM(unsigned char LPM_level){
	if (LPM_level == 0x00) 
	  _BIS_SR(LPM0_bits);     /* Enter Low Power Mode 0 */
        else if(LPM_level == 0x01) 
	  _BIS_SR(LPM1_bits);     /* Enter Low Power Mode 1 */
        else if(LPM_level == 0x02) 
	  _BIS_SR(LPM2_bits);     /* Enter Low Power Mode 2 */
	else if(LPM_level == 0x03) 
	  _BIS_SR(LPM3_bits);     /* Enter Low Power Mode 3 */
        else if(LPM_level == 0x04) 
	  _BIS_SR(LPM4_bits);     /* Enter Low Power Mode 4 */
}
//---------------------------------------------------------------------
//            Enable interrupts
//---------------------------------------------------------------------
void enable_interrupts(){
  _BIS_SR(GIE);
}
//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts(){
  _BIC_SR(GIE);
}
//---------------------------------------------------------------------
void state1TIMERconfig(void){
	TA1CTL = TASSEL_2+MC_2+TAIE;
	TA1CCTL2 = CM_1+CCIS_0+SCS+CAP+CCIE;
	TA1CCTL2 &= ~(CCIFG);
}
//---------------------------------------------------------------------
void state2TIMERconfig(void){
	TA0CTL = TASSEL_2+ID_3+MC_3;            //Smclk+Div8+UpDownMode+TAIE 
	TA0CCR0 = 0xFFFF;                                  // set 2^16 value
	TA0CCTL0 = CCIE;
	TA0R = 0xFFFF;
}
//---------------------------------------------------------------------
void state3TIMERconfig(void){
	TA1CTL = TASSEL_2+MC_1;            //Smclk+UpMode
	TA1CCTL1 = OUTMOD_6;               //Toggle/Set+CCIE+
	TA1CCR0 = 1056;					   //Period of 1kHz PWM
	TA1CCR1 = 1056/2;				   //Peirod of '1' output
}
//---------------------------------------------------------------------
void haltTA0(void){
	TA0CTL &= ~(MC_3+TAIE);		//halt clock+disable TA0IE
	TA0CCTL0 &= ~CCIE;          //disable TA0CCR0 IE
}
//---------------------------------------------------------------------
void haltTA1(void){
	TA1CTL &= ~(MC_3+TAIE);		//halt clock+disable TA0IE
	TA1CCTL2 &= ~CCIE;          //disable TA1CCR2 IE
}
//---------------------------------------------------------------------
void startTA0_st2(void){
	TA0CTL |= MC_3;    //UpDown mode
	TA0CCTL0 |= CCIE;
}
//---------------------------------------------------------------------
void SetFreqByADC(int Nadc){
	res =-m*Nadc+n;		//res in Q11.5
	res = res / 32;     //inverse to normal representation      
	TA1CCR0 = res;					   //Period of PWM
	TA1CCR1 = res/2;				   //Peirod of '1' output	
}
//---------------------------------------------------------------------
void state3ADCconfig(){
	ADC10CTL0 = ADC10SHT_2+ADC10ON+ADC10IE;            //16xADC10CLKS+ADC10ON+ADC10IE
	ADC10CTL1 = INCH_3+ADC10SSEL_0+CONSEQ_0;               //ChannelA3+ADC10OSCreference+SingleChannelSingleConversion
	ADC10AE0 =  BIT3;            //enable A3 Channel
}
int readADC10MEM(){
	return ADC10MEM;
}
void haltADC(){
	ADC10CTL0 &= ~(ADC10ON+ENC+ADC10IE);  
}
void sampleADC(){
	ADC10CTL0 |= ADC10SC+ENC;
}
//******************************************************************
// initialize the LCD
//******************************************************************
void lcd_init(){
  
	char init_value;

	if (LCD_MODE == FOURBIT_MODE) init_value = 0x3 << LCD_DATA_OFFSET;
        else init_value = 0x3F;
	
	LCD_RS_DIR(OUTPUT_PIN);
	LCD_EN_DIR(OUTPUT_PIN);
	LCD_RW_DIR(OUTPUT_PIN);
        LCD_DATA_DIR |= OUTPUT_DATA;
        LCD_RS(0);
	LCD_EN(0);
	LCD_RW(0);
        
	DelayMs(15);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();
	DelayMs(5);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();
	DelayUs(200);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();
	
	if (LCD_MODE == FOURBIT_MODE){
		LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
		LCD_DATA_WRITE |= 0x2 << LCD_DATA_OFFSET; // Set 4-bit mode
		lcd_strobe();
		lcd_cmd(0x28); // Function Set
	}
        else lcd_cmd(0x3C); // 8bit,two lines,5x10 dots 
	
	lcd_cmd(0xF); //Display On, Cursor On, Cursor Blink
	lcd_cmd(0x1); //Display Clear
	lcd_cmd(0x6); //Entry Mode
	lcd_cmd(0x80); //Initialize DDRAM address to zero
}
//******************************************************************
// send a command to the LCD
//******************************************************************
void lcd_cmd(unsigned char c){
  
	LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

	if (LCD_MODE == FOURBIT_MODE)
	{
		LCD_DATA_WRITE &= ~OUTPUT_DATA;// clear bits before new write
                LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;
		lcd_strobe();
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
    		LCD_DATA_WRITE |= (c & (0x0F)) << LCD_DATA_OFFSET;
		lcd_strobe();
	}
	else
	{
		LCD_DATA_WRITE = c;
		lcd_strobe();
	}
}
//******************************************************************
// send data to the LCD
//******************************************************************
void lcd_data(unsigned char c){
        
	LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

	LCD_DATA_WRITE &= ~OUTPUT_DATA;       
	LCD_RS(1);
	if (LCD_MODE == FOURBIT_MODE)
	{
    		LCD_DATA_WRITE &= ~OUTPUT_DATA;
                LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;  
		lcd_strobe();		
                LCD_DATA_WRITE &= (0xF0 << LCD_DATA_OFFSET) | (0xF0 >> 8 - LCD_DATA_OFFSET);
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
		LCD_DATA_WRITE |= (c & 0x0F) << LCD_DATA_OFFSET; 
		lcd_strobe();
	}
	else
	{
		LCD_DATA_WRITE = c;
		lcd_strobe();
	}
          
	LCD_RS(0);   
}
//******************************************************************
// write a string of chars to the LCD
//******************************************************************
void lcd_puts(const char * s){
  
	while(*s)
		lcd_data(*s++);
}
//******************************************************************
// Delay usec functions
//******************************************************************
void DelayUs(unsigned int cnt){
  
	unsigned char i;
        for(i=cnt ; i>0 ; i--) asm("nop"); // tha command asm("nop") takes raphly 1usec
	
}
//******************************************************************
// Delay msec functions
//******************************************************************
void DelayMs(unsigned int cnt){
  
	unsigned char i;
        for(i=cnt ; i>0 ; i--) DelayUs(1000); // tha command asm("nop") takes raphly 1usec
	
}
//******************************************************************
// lcd strobe functions
//******************************************************************
void lcd_strobe(){
  LCD_EN(1);
  asm("nop");
  asm("nop");
  LCD_EN(0);
}
//*********************************************************************
//            Port1 Interrupt Service Rotine
//*********************************************************************
#pragma vector=PORT1_VECTOR
  __interrupt void PBs_handler(void){
   
	delay(debounceVal);
//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
	if(PBsArrIntPend & PB0){
	  state = state1;
	  PBsArrIntPend &= ~PB0;
        }
    else if(PBsArrIntPend & PB1){
	  state = state2;
	  PBsArrIntPend &= ~PB1; 
        }
	else if(PBsArrIntPend & PB2){ 
	  state = state3;
	  PBsArrIntPend &= ~PB2;
        }
	else if(PBsArrIntPend & PB3){ 
// pull-down mode	  state = state4;
	  PBsArrIntPend &= ~PB3;
        }	
//---------------------------------------------------------------------
//            Exit from a given LPM 
//---------------------------------------------------------------------	
        switch(lpm_mode){
		case mode0:
		 LPM0_EXIT; // must be called from ISR only
		 break;
		 
		case mode1:
		 LPM1_EXIT; // must be called from ISR only
		 break;
		 
		case mode2:
		 LPM2_EXIT; // must be called from ISR only
		 break;
                 
                case mode3:
		 LPM3_EXIT; // must be called from ISR only
		 break;
                 
                case mode4:
		 LPM4_EXIT; // must be called from ISR only
		 break;
	}      
}
//*********************************************************************
//            TA1 Interrupt Service Rotine
//*********************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) TIMER1_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(TA1IV,0x0A))
  {
      case  TA1IV_NONE: break;              // Vector  0:  No interrupt
      case  TA1IV_TACCR1: break;            // Vector  2:  TACCR1 CCIFG
      case  TA1IV_TACCR2:                   // Vector  4:  TACCR2 CCIFG
          //if (TA1CCTL2 & CCI){              // Capture Input Pin Status
                  val1 = val2; 
                  val2 = TA1CCR2;
                  LPM0_EXIT;
                  break;
             // }
      break;
      case TA1IV_TAIFG: break;              // Vector 10:  TAIFG
      default: 	break;
  }
}
//*********************************************************************
//            TimerA0 Interrupt Service Rotine
//*********************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
  LPM0_EXIT;
}
//*********************************************************************
//            ADC10 Interrupt Service Rotine
//*********************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
  LPM0_EXIT;
}