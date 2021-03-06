 /******************************************************************************
 *
 * File Name: motor.c
 *
 * Created on: Feb 15, 2021
 *
 * Author: Basmala Magdy
 *
 *******************************************************************************/


#include "adc.h"
#include "lcd.h"
#include "motor.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Running the Motor
 * */

void PWM_Timer0_Init(unsigned char set_duty_cycle)
{
	Timer0_Initial_value = 0;
	Compare_Value  = set_duty_cycle;
	MOTOR_OUT_PORT_DIR |= (1<<EN1);
	/*
	 * prescaler = 8 --> CS01 = 1
	 * wave generator mode fast pmw ---> WGM00 , WGM01 = 1
	 * fast PWM non inverting mode --> COM01 = 1*/
	Control_Value = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);

}

/*
 * Running the interrupt1 with raising it should revert the motor direction whatever the speed of the motor
 * */
void INT1_reverse(void)
{
	INT1_MCU_Control |= (1 << ISC10) | (1 << ISC11);                   /* Raising edge*/
	General_INT1_Control |= (1 << INT1);                                       /* GENERATE INT1*/
}

ISR(INT1_vect)
{
	TOGGLE_BIT(MOTOR_OUT_PORT,IN1);
	TOGGLE_BIT(MOTOR_OUT_PORT,IN2);
}

