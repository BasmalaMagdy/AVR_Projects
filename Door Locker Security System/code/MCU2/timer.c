/********************************************************************************
 * File Name: timer.c
 *
 * Author: Basmala Magdy
 *
 * Created on: Mar 30, 2021
 *
 * Description: timer driver
 *******************************************************************************/

#include "timer.h"
//#include "micro_config.h"

/*********************************************************************************************************************
 *                                                      Global Variables                                             *
 *********************************************************************************************************************/

/*Global variables to get the addresses of the call back functions in the application*/
static volatile void (*g_t0_ovf_callBackPtr)(void) = NULL_PTR; /*Timer0,overflow mode*/
static volatile void (*g_t0_ctc_callBackPtr)(void) = NULL_PTR; /*Timer0,compare mode*/
static volatile void (*g_t1_ovf_callBackPtr)(void) = NULL_PTR; /*Timer1,overflow mode*/
static volatile void (*g_t1_ctc_callBackPtr)(void) = NULL_PTR; /*Timer1,compare mode*/
static volatile void (*g_t2_ovf_callBackPtr)(void) = NULL_PTR; /*Timer2,overflow mode*/
static volatile void (*g_t2_ctc_callBackPtr)(void) = NULL_PTR; /*Timer2,compare mode*/

static volatile uint8 prescalar;
/*********************************************************************************************************************
 *                                                Interrupt Service Routines                                         *
 *********************************************************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_t0_ovf_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after each overflow */
		(*g_t0_ovf_callBackPtr)();
		/* another method to call the function using pointer to function t0_ovf_callBackPtr(); */
	}
}
ISR(TIMER0_COMP_vect)
{
	if(g_t0_ctc_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application
		 * after the timer reach the compare value
		 */
		(*g_t0_ctc_callBackPtr)();
		/* another method to call the function using pointer to function t0_ctc_callBackPtr(); */
	}
}
ISR(TIMER1_OVF_vect)
{
	if(g_t1_ovf_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after each overflow */
		(*g_t1_ovf_callBackPtr)();
		/* another method to call the function using pointer to function t1_ovf_callBackPtr(); */
	}
}
ISR(TIMER1_COMPA_vect)
{
	if(g_t1_ctc_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application
		 * after the timer reach the compare value
		 */
		(*g_t1_ctc_callBackPtr)();
		/* another method to call the function using pointer to function t1_ctc_callBackPtr(); */
	}
}
ISR(TIMER2_OVF_vect)
{
	if(g_t2_ovf_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after each overflow */
		(*g_t2_ovf_callBackPtr)();
		/* another method to call the function using pointer to function t2_ovf_callBackPtr(); */
	}
}
ISR(TIMER2_COMP_vect)
{
	if(g_t2_ctc_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application
		 * after the timer reach the compare value
		 */
		(*g_t2_ctc_callBackPtr)();
		/* another method to call the function using pointer to function t2_ctc_callBackPtr(); */
	}
}

/*********************************************************************************************************************
 *                                                Functions Definitions                                        *
 *********************************************************************************************************************/
/*
 * Function that initialize the timer
 * Given timer and the mode, it initiate the registers
 * the initial value and the compare value
 */
void Timer_init(const Timer_ConfigType * Config_Ptr)
{

	if(Config_Ptr->type  == TIMER0)
	{
		TCNT0 = Config_Ptr->s_init; /*Put the initial value in TCNT0*/
		TCCR0 = (1<<FOC0); /*Non PWM mode FOC0=1*/
		TCCR0 = (TCCR0 & 0xF8) | (prescalar);

		if(Config_Ptr->mode == OVERFLOW)
		{
			TIMSK |= (1<<TOIE0); // Enable Timer0 Overflow Interrupt
			/* Configure the timer control register
			 * 2. Normal Mode WGM01=0 & WGM00=0
			 * 3. Normal Mode COM00=0 & COM01=0
			 */
		}
		else if(Config_Ptr->mode == COMPARE)
		{
			OCR0  = Config_Ptr->s_compare; /* Set Compare Value*/
			TIMSK |= (1<<OCIE0); // Enable Timer0 Compare Interrupt
			/* Configure timer0 control register
			 * 2. CTC Mode WGM01=1 & WGM00=0
			 * 3. No need for OC0 in this example so COM00=0 & COM01=0
			 */
			TCCR0 = (1<<WGM01);
		}
	}
	else if(Config_Ptr->type  == TIMER1)
	{
		TCCR1A = (1<<FOC1A) | (1<<FOC1B);
		TCCR1B = (TCCR1B & 0xF8) | (prescalar); /*Put the prescalar in the first 3-bits*/
		TCNT1 = Config_Ptr->s_init;
		if(Config_Ptr->mode == OVERFLOW)
		{
			TIMSK = (1<<TOIE1); /*TOIE1=1: Enable overflow interrupt*/
		}
		else if(Config_Ptr->mode == COMPARE)
		{
			TCCR1B = (1<<WGM12); /*WGM12=1: Compare mode (Mode 12)*/
			OCR1A = Config_Ptr->s_compare; /* compare value in OCR1A mode 4*/
			TIMSK = (1<<OCIE1A); /*OCIE1A=1: Enable output compare A match interrupt*/
		}
	}
	else if(Config_Ptr->type  == TIMER2)
	{
		TCCR2= (1<<FOC2);
		TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->prescalar);
		TCNT2 = Config_Ptr->s_init;
		if(Config_Ptr->mode == OVERFLOW)
		{
			TIMSK = (1<<TOIE2); /*TOIE1=2: Enable overflow interrupt*/
		}
		else if(Config_Ptr->mode == COMPARE)
		{
			TCCR2 |= (1<<WGM21);
			OCR2 = Config_Ptr->s_compare;
			TIMSK = (1<<OCIE2);
		}
	}


	if((Config_Ptr->prescalar) == NO_CLOCK)
	{
		prescalar = 0;/*If it is NO_CLOCK , it should be 0*/
	}
	else if((Config_Ptr->prescalar) == F_CPU_CLOCK)
	{
		prescalar = 1;/*If it is F_CPU_CLOCK , it should be 1*/
	}
	else if((Config_Ptr->prescalar) == F_CPUt_8)
	{
		prescalar = 2;/*If it is F_CPUt_8, it should be 2*/
	}
	else if((Config_Ptr->prescalar) == F_CPUt_64)
	{
		prescalar = 3;/*If it is F_CPUt_8, it should be 3*/
	}
	else if((Config_Ptr->prescalar) == F_CPUt_256)
	{
		prescalar = 4;/*If it is F_CPUt_256 , it should be 4*/
	}
	else if((Config_Ptr->prescalar) == F_CPUt_1024)
	{
		prescalar = 5;/*If it is F_CPUt_1024, it should be 5*/
	}
}
/*Function to stop timer*/
void Timer_stop(void)
{
	TCCR0 = (TCCR0 & 0xF8) | (0);
}
/*Function to start timer*/
void Timer_start(void)
{
	TCNT0 = 0;
	TCCR0 = (TCCR0 & 0xF8) | (prescalar);
}

/*Function to set the Call Back function address for Timer0, overflow mode*/
void Timer0_Ovf_setCallBack(void(*a_t0_ovf_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t0_ovf_callBackPtr = a_t0_ovf_ptr;
}

/*Function to set the Call Back function address for Timer0, compare mode*/
void Timer0_CTC_setCallBack(void(*a_t0_ctc_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t0_ctc_callBackPtr = a_t0_ctc_ptr;
}

/*Function to set the Call Back function address for Timer1, overflow mode*/
void Timer1_Ovf_setCallBack(void(*a_t1_ovf_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t1_ovf_callBackPtr = a_t1_ovf_ptr;
}

/*Function to set the Call Back function address for Timer1, compare mode*/
void Timer1_CTC_setCallBack(void(*a_t1_ctc_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t1_ctc_callBackPtr = a_t1_ctc_ptr;
}

/*Function to set the Call Back function address for Timer2, overflow mode*/
void Timer2_Ovf_setCallBack(void(*a_t2_ovf_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t2_ovf_callBackPtr = a_t2_ovf_ptr;
}

/*Function to set the Call Back function address for Timer2, compare mode*/
void Timer2_CTC_setCallBack(void(*a_t2_ctc_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t2_ctc_callBackPtr = a_t2_ctc_ptr;
}










