 /******************************************************************************
 *
 * Module: Common - Platform Types Abstraction
 *
 * File Name: duty.h
 *
 * Description: file to calculate duty cycle
 *
 * Author: Basmala Magdy
 *
 *******************************************************************************/

#define F_CPU 8000000UL
#include "lcd.h"
/*********************************************************************************************************************
 *                                                      Global Variables                                             *
 *********************************************************************************************************************/
uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;
uint16 g_timePeriod = 0;
uint16 g_timePeriodPlusHigh = 0;

void Timer_Init(void){
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);  /*Non PWM mode FOC0=1*/
	TCCR1B = (1<<CS10); //N = 1
	TCNT1 = 0;//initial value
}
/* External INT0 Interrupt Service Routine */
ISR(INT0_vect)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		TCNT1 = 0;
		/* Detect falling edge */
		TOGGLE_BIT(MCUCR,ISC00);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = TCNT1;

		/* Detect rising edge */
		TOGGLE_BIT(MCUCR,ISC00);
	}
	else if(g_edgeCount == 3)
	{
		/* Store the Period time value */
		g_timePeriod = TCNT1;

		/* Detect falling edge */
		TOGGLE_BIT(MCUCR,ISC00);
	}
	else if(g_edgeCount == 4)
	{
		/* Store the Period time value + High time value */
		g_timePeriodPlusHigh = TCNT1;

		/* Clear the timer counter register to start measurements again */

		/* Detect rising edge */
		TOGGLE_BIT(MCUCR,ISC00);
	}
}

int main()
{
	uint32 dutyCycle = 0;

	/* Initialize both the LCD driver */
	LCD_init();
	/*CALL time init function */
	Timer_Init();

	SREG  &= ~(1<<7);                 /* Disable interrupts by clearing I-bit*/
	DDRD  &= (~(1<<PD2));               /* Configure INT0/PD2 as input pin*/
	GICR  |= (1<<INT0);                  /* Enable external interrupt pin INT0*/
	MCUCR |= (1<<ISC00) | (1<<ISC01);    /*Trigger INT0 with the raising edge*/
	SREG  |= (1<<7);                      /* Enable interrupts by setting I-bit*/

	while(1)
	{
		if(g_edgeCount == 4)
		{
			GICR  &= ~(1<<INT0); /* Disable ICU Driver */
			g_edgeCount = 0;
			LCD_displayString("Duty = ");
			/* calculate the dutyCycle */
			dutyCycle = ((float)(g_timePeriodPlusHigh-g_timePeriod) / (g_timePeriodPlusHigh - g_timeHigh)) * 100;
			/* display the dutyCycle on LCD screen */

			LCD_intgerToString(dutyCycle);
			LCD_displayCharacter('%');
		}
	}
}
































///*
// * duty.c
// *
// *  Created on: Apr 3, 2021
// *      Author: hs
// */
//
//
//#include "micro_config.h"
//
//uint8 g_Interrupt_Flag = 0;
//uint8 g_edgeCount = 0;
//uint16 g_timeHigh = 0;
//uint16 g_timePeriod = 0;
//uint16 g_timePeriodPlusHigh = 0;
//
//
//void Timer_Init(void){
//	TCCR1A = (1<<FOC1A) | (1<<FOC1B); // Non PWM
//	TCCR1B = (1<<CS10); //N = 1024 & enable compare mode
//	TCNT1 = 0;//initial value
//}
//
//ISR(INT0_vect)
//{
//	// set the interrupt flag to indicate that INT1 occurs
//	//g_Interrupt_Flag = 1;
//   // Trigger INT0 with the raising edge
//	g_edgeCount++;
//		if(g_edgeCount == 1)
//		{
//			TCNT1 = 0;
//			TOGGLE_BIT(MCUCR,ISC00);
//
//		}
//		else if(g_edgeCount == 2)
//		{
//			g_timeHigh = TCNT1;
//			TOGGLE_BIT(MCUCR,ISC00);
//		}
//		else if(g_edgeCount == 3)
//		{
//			g_timePeriod = TCNT1;
//			TOGGLE_BIT(MCUCR,ISC00);
//
//		}
//		else if(g_edgeCount == 4)
//		{
//
//			g_timePeriodPlusHigh = TCNT1;
//			TOGGLE_BIT(MCUCR,ISC00);
//			TCNT1 = 0;
//		}
//}
////void INT0_Init(void)
////{
////
////	                // Enable external interrupt pin INT0
////	                // Trigger INT0 with the raising edge
////	g_edgeCount++;
////		if(g_edgeCount == 1)
////		{
////			TCNT1 = 0;
////			TOGGLE_BIT(MCUCR,ISC00);
////
////		}
////		else if(g_edgeCount == 2)
////		{
////			g_timeHigh = TCNT1;
////			TOGGLE_BIT(MCUCR,ISC00);
////
////		}
////		else if(g_edgeCount == 3)
////		{
////			g_timePeriod = TCNT1;
////			TOGGLE_BIT(MCUCR,ISC00);
////
////		}
////		else if(g_edgeCount == 4)
////		{
////
////			g_timePeriodPlusHigh = TCNT1;
////			TOGGLE_BIT(MCUCR,ISC00);
////			TCNT1 = 0;
////		}
////
////}
//
//
//int main()
//{
//
//	uint32 dutyCycle = 0;
//	//INT0_Init();
//
//
////	Timer_ConfigType Timer_Config = {TIMER1,OVERFLOW,F_CPU_CLOCK,0,0};
//
////	Timer_init(& Timer_Config);   /* initialize TIMER */
//	Timer_Init();
//
//	//Timer1_Ovf_setCallBack(INT0_Init);
//	LCD_init();
//	//Timer_start();
//	/* Enable Global Interrupt I-Bit */
//
////	SREG |= (1<<7);
////	GICR  |= (1<<INT0);                 // Enable external interrupt pin INT0
////	MCUCR |= (1<<ISC01) | (1<<ISC00);
//
//	SET_BIT(MCUCR,ISC01);
//	SET_BIT(MCUCR,ISC00);
//	SET_BIT(GICR,INT0);
//	SET_BIT(SREG,7);
//	CLEAR_BIT(DDRD, 2);
//
//	while(1)
//	{
//
//
//		//if(g_Interrupt_Flag = 1)
//		//{
//
//			if(g_edgeCount == 4)
//			{
//				CLEAR_BIT(GICR, INT0);
//				g_edgeCount = 0;
//				LCD_clearScreen();
//				LCD_goToRowColumn(0,0);
//				LCD_displayString("Duty = ");
//				/* calculate the dutyCycle */
//				dutyCycle = ((float)(g_timePeriodPlusHigh-g_timePeriod) / (g_timePeriodPlusHigh - g_timeHigh)) * 100;
//				/* display the dutyCycle on LCD screen */
//				LCD_goToRowColumn(1,0);
//				LCD_intgerToString(dutyCycle);
//				LCD_displayCharacter('%');
//			}
//		//}
//	}
//
//
//}
