/*
 * File        : MTIMER0_Program.c
 * Description : This file includes TIMER0 Driver test for Atmega32
 * Author      : Ahmed Elmogy Embedded SW Engineer 
 * Date        : 20/2/2020
 * Git account : https://github.com/AhmedElmougy
 * mail        : ahmed.m.elmogy@gmil.com 
 */

#include "LSTD_Types.h"
#include "MDIO_Interface.h"
#include "MTIMER0_Interface.h"
#include "HLED_Interface.h"

#define F_CPU 8000000UL
#include <util/delay.h>


/*
 * Choose Mode to test
 * and choose the same mode in Config file
 */
//#define OV
//#define CTC
#define FAST_PWM
//#define PHASE_PWM

/*Call back functions fr ISRs*/
void timer0OverflowISR(void);
void timer0CTCISR(void);

int main(void)
{
	/*set PB3 output*/
	MDIO_u8SetDirectionBit(MDIO_PORTB,MDIO_PB3,MDIO_OUTPUT);
	
	/*Initialize timer0*/
	MTIMER0_voidInitializeTimer0();
	
	/*==============================================================
	 * CTC
	 *==============================================================*/
	#ifdef CTC
	MTIMER0_voidCTCCallBackfunction(timer0CTCISR);
	MTIMER0_voidStartTimer();
	#endif
	/*==============================================================
	 * Over Flow
	 *==============================================================*/
	#ifdef OV
	MTIMER0_voidOverFlowCallBack(timer0OverflowISR);
	MTIMER0_voidSetDesiredTime_ms(100);
	#endif
	
	while (1)
	{
		/*==============================================================
		 * Fast PWM
		 *==============================================================*/
		 #ifdef FAST_PWM
		 for (u8 i=0;i<100;i++)
		 {
			 MTIMER0_voidFastPWM(i);
			 _delay_ms(50);
		 }
		 
		 #endif
		/*==============================================================
		 * Phase correct
		 *==============================================================*/
		 #ifdef PHASE_PWM
		 for (u8 i=0;i<100;i++)
		 {
			 MTIMER0_voidPhaseCorrectPWM(i);
			 _delay_ms(50);
		 }
		 
		 #endif
	}
}

/*Toggle PB3 each time ISR entered*/
void timer0OverflowISR(void)
{
	MDIO_u8ToggleBit(MDIO_PORTB,MDIO_PB3);
}

/*Toggle PB3 each 1000 ms*/
void timer0CTCISR(void)
{
	static u16 count_us = 0;
	static u16 count_ms = 0;
	count_us++;
	if (4 == count_us)
	{
		count_ms++;
		count_us=0;
	}
	
	if (count_ms == 1000)
	{
		
		MDIO_u8ToggleBit(MDIO_PORTB,MDIO_PB3);
		count_ms =0;
	}
}