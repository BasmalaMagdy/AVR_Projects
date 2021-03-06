 /******************************************************************************
 *
 * File Name: potentiometer.c
 *
 * Created on: Feb 15, 2021
 *
 * Author: Basmala Magdy
 *
 *******************************************************************************/


#include "adc.h"
#include "lcd.h"
#include "motor.h"


int main(void)
{
/************************************************************************
 *                            Local variable
 * ********************************************************************/
	uint32 value;
	uint32 motor_value;
/************************* Enable Global Interrupts ******************/
	SET_BIT(SREG,7);

	LCD_init();                                     /* initialize LCD driver */
	ADC_init();                                    /* initialize ADC driver */
	LCD_clearScreen();                            /* clear LCD at the beginning */
/* display this string "ADC Value = " only once at LCD */
	LCD_displayString("ADC Value = ");


	MOTOR_OUT_PORT_DIR |= ((1<<IN1) | (1<<IN2));
	MOTOR_OUT_PORT &= ((~(1<<IN1)) & (~(1<<IN2)));

	MOTOR_OUT_PORT &= (~(1 << IN1));
	MOTOR_OUT_PORT |= (1 << IN2);

	while(1)
	{
		/******************************************************************
		 * Display LCD
		 ***************************************************************** */
		LCD_goToRowColumn(0,11);          /* display the number every time at this position */
	    ADC_readChannel(0);               /* read channel zero where the potentiometer is connect */
		value = ((uint32)g_adcResult);    /* put ADC value in variable */
		LCD_intgerToString(value);        /* display the ADC value on LCD screen */

		/******************************************************************
		 * Display MOTOR
		 ***************************************************************** */
		motor_value = ((uint32)g_adcResult)/4;                 /* calculate the Motor_speed from the ADC value*/
		/*motor_value = ((255*((uint32)g_adcResult))/1023);*/
		PWM_Timer0_Init(motor_value);                           /* send motor_speed to PWM function */
		INT1_reverse();                                         /*reverse the direction of motor*/
	}


}
