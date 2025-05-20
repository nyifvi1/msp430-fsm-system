#ifndef _halGPIO_H_
#define _halGPIO_H_

#include  "../header/bsp.h"    		// private library - BSP layer
#include  "../header/app.h"    		// private library - APP layer


extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable

extern void sysConfig(void);
extern unsigned char readSWs(void);
extern void delay(unsigned int);
extern void enterLPM(unsigned char);
extern void enable_interrupts();
extern void disable_interrupts();
extern void state1TIMERconfig(void);
extern void state2TIMERconfig(void);
extern void state3TIMERconfig(void);
extern void haltTA0(void);
extern void haltTA1(void);
extern void startTA0_st2(void);

extern void SetFreqByADC(int Nadc);
extern void state3ADCconfig();
extern int readADC10MEM();
extern void haltADC();
extern void sampleADC();

extern void lcd_init(void);
extern void lcd_init(); 
extern void lcd_cmd(unsigned char);
extern void lcd_data(unsigned char);
extern void lcd_puts(const char * s);
extern void lcd_strobe();
extern void DelayMs(unsigned int);
extern void DelayUs(unsigned int);
extern __interrupt void PBs_handler(void);

#endif







