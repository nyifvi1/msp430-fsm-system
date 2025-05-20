#ifndef _bsp_H_
#define _bsp_H_

#include  <msp430g2553.h>          // MSP430x2xx
// #include  <msp430xG46x.h>  // MSP430x4xx


#define   debounceVal      250
#define   LEDs_SHOW_RATE   0xFFFF  // 62_5ms
#define   Fsmclk           0x10295F // Tsmlk=1059167 Hz

// LEDs abstraction
//#define LEDsArrPort        P1OUT
//#define LEDsArrPortDir     P1DIR
//#define LEDsArrPortSel     P1SEL

// Switches abstraction
#define SWsArrPort         P2IN
#define SWsArrPortDir      P2DIR
#define SWsArrPortSel      P2SEL
#define SWmask             0x01
#define SW0                0x01

// PushButtons abstraction
#define PBsArrPort	       P1IN 
#define PBsArrIntPend	   P1IFG 
#define PBsArrIntEn	       P1IE
#define PBsArrIntEdgeSel   P1IES
#define PBsArrPortSel      P1SEL 
#define PBsArrPortDir      P1DIR 
#define PB0                0x01
#define PB1                0x02
#define PB2                0x04

//PB3 abstraction
#define PB3                0x01

//Analog abstraction
#define AnalogPin          0x08

extern void GPIOconfig(void);
extern void TIMERconfig(void);
extern void ADCconfig(void);
/*----------------------------------------------------------
  LCD
------------------------------------------------------------*/
#define LCD_EN(a)	(!a ? (P2OUT&=~0X20) : (P2OUT|=0X20)) // P2.5 is lcd enable pin
#define LCD_EN_DIR(a)	(!a ? (P2DIR&=~0X20) : (P2DIR|=0X20)) // P2.5 pin direction 

#define LCD_RS(a)	(!a ? (P2OUT&=~0X40) : (P2OUT|=0X40)) // P2.6 is lcd RS pin
#define LCD_RS_DIR(a)	(!a ? (P2DIR&=~0X40) : (P2DIR|=0X40)) // P2.6 pin direction  
  
#define LCD_RW(a)	(!a ? (P2OUT&=~0X80) : (P2OUT|=0X80)) // P2.7 is lcd RW pin
#define LCD_RW_DIR(a)	(!a ? (P2DIR&=~0X80) : (P2DIR|=0X80)) // P2.7 pin direction

#define LCD_DATA_OFFSET 0x04 //data pin selection offset for 4 bit mode, variable range is 0-4, default 0 - Px.0-3, 4 offset
   
#define LCD_DATA_WRITE	P1OUT
#define LCD_DATA_DIR	P1DIR
#define LCD_DATA_READ	P1IN
#define LCD_DATA_SEL    P1SEL
#define LCD_CNTL_SEL    P2SEL

#define FOURBIT_MODE	0x0
#define EIGHTBIT_MODE	0x1
#define LCD_MODE        FOURBIT_MODE
   
#define OUTPUT_PIN      1	
#define INPUT_PIN       0	
#define OUTPUT_DATA     (LCD_MODE ? 0xFF : (0x0F << LCD_DATA_OFFSET))
#define INPUT_DATA      0x00	

#define LCD_STROBE_READ(value)	LCD_EN(1), \
				asm("nop"), asm("nop"), \
				value=LCD_DATA_READ, \
				LCD_EN(0) 

#define	lcd_cursor(x)			lcd_cmd(((x)&0x7F)|0x80)
#define lcd_clear()				lcd_cmd(0x01)
#define lcd_putchar(x)			lcd_data(x)
#define lcd_goto(x)				lcd_cmd(0x80+(x))
#define lcd_cursor_right()		lcd_cmd(0x14)
#define lcd_cursor_left()		lcd_cmd(0x10)
#define lcd_display_shift()		lcd_cmd(0x1C)
#define lcd_home()				lcd_cmd(0x02)
#define cursor_off              lcd_cmd(0x0C)
#define cursor_on               lcd_cmd(0x0F) 
#define lcd_function_set        lcd_cmd(0x3C) // 8bit,two lines,5x10 dots 
#define lcd_new_line            lcd_cmd(0xC0)                                  

// #define CHECKBUSY	1  // using this define, only if we want to read from LCD

#ifdef CHECKBUSY
	#define	LCD_WAIT lcd_check_busy()
#else
	#define LCD_WAIT DelayMs(5)
#endif

#endif



