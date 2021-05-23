/********************************************************************************
 * File Name: timer.h
 *
 * Author: Basmala Magdy
 *
 * Created on: Mar 30, 2021
 *
 * Description: timer driver
 *******************************************************************************/


#ifndef TIMER_H_
#define TIMER_H_

#include "micro_config.h"

   /*------------------------------------------- Structure Declaration  ----------------------------------------------*/

typedef enum
{
	TIMER0,TIMER1,TIMER2
}Timer_Type;
typedef enum
{
	OVERFLOW,COMPARE
}Timer_Mode;
typedef enum
{
	NO_CLOCK,F_CPUt_CLOCK,F_CPUt_8,F_CPUt_32,F_CPUt_64,F_CPUt_128,F_CPUt_256,F_CPUt_1024
}Timer_Prescalar;
typedef struct
{
	Timer_Type type;
	Timer_Mode mode;
	Timer_Prescalar prescalar;
	uint16 s_init; /*Take the initial value from the user*/
	uint16 s_compare; /*Take the compare value from user, put 0 if using overflow mode*/
}Timer_ConfigType;

/*******************************************************************************************************************
 *                                                Functions Prototypes                                             *
 ********************************************************************************************************************/



void Timer_init(const Timer_ConfigType * Config_Ptr);

/*Function to set the Call Back function address for Timer0, overflow mode*/
void Timer0_Ovf_setCallBack(void(*a_t0_ovf_ptr)(void));

/*Function to set the Call Back function address for Timer0, compare mode*/
void Timer0_CTC_setCallBack(void(*a_t0_ctc_ptr)(void));

/*Function to set the Call Back function address for Timer1, overflow mode*/
void Timer1_Ovf_setCallBack(void(*a_t1_ovf_ptr)(void));

/*Function to set the Call Back function address for Timer1, compare mode*/
void Timer1_CTC_setCallBack(void(*a_t1_ctc_ptr)(void));

/*Function to set the Call Back function address for Timer2, overflow mode*/
void Timer2_Ovf_setCallBack(void(*a_t2_ovf_ptr)(void));

/*Function to set the Call Back function address for Timer2, compare mode*/
void Timer2_CTC_setCallBack(void(*a_t2_ctc_ptr)(void));

void Timer_stop();

void Timer_start();





#endif /* TIMER_H_ */
