/********************************************************************************
 * File Name: MCU11.c
 *
 * Author: Basmala Magdy
 *
 * Created on: Mar 30, 2021
 *
 * Description: Main file of MCU11 for Final Project
 *******************************************************************************/


/*******************************************************************************
 *                                   INCLUDES                                  *
 *******************************************************************************/
#include "micro_config.h"

/*******************************************************************************
 *                                  Definitions                                *
 *******************************************************************************/
#define M1_READY 0x01              /*MCU1 sends to MCU2 that it is ready to start*/
#define INIT_FLAG 0x0A             /*Flag is saved if the one run code is done*/

/*******************************************************************************
 *                                 MAIN FUNCTION                              *
 *******************************************************************************/
int main(void)
{
	uint8 init_flag =0;      /*variable used to know if the first code is done before or not*/
	SREG |= (1<<7);          /* Enable Global Interrupt I-Bit */
	/*
	 *  the structure of timer
	 * 1. Timer_type: Timer0
	 * 2. Timer_mode: Overflow mode
	 * 3. Prescalar: clk/1024
	 * 4. Initial value: 0
	 * 5. Compare value: 0  (overflow mode)
	 */
	Timer_ConfigType Timer_Config = {TIMER0,OVERFLOW,F_CPU_1024,0,0};
	/*
	 * the structure of UART
	 * 1. Parity: Disabled (no parity)
	 * 2. Stop_Bit: BIT_1 (1 stop bit)
	 * 3. CharSize: BIT_8 (8-bits data)
	 * 4. Baudrate: 9600
	 */
	UART_ConfigType UART_Config = {DISABLED,BIT_1,BIT_8,9600};

	/*Function to set the Call Back function address for Timer0, overflow mode in time driver
	 * Calc_time to Calculate number of seconds
	 * */
	Timer0_Ovf_setCallBack(Calc_time);


	LCD_init();                   /* initialize LCD */
	UART_init(& UART_Config);    /* initialize UART */
	Timer_init(& Timer_Config);   /* initialize TIMER */


	UART_sendByte(M1_READY); /*MC1 sends to MC2 that it is ready to start*/
	/*
	 * variable used to receive initialization value that be saved in the address
	 * to know if the first code is done before or not
	 */
	init_flag = UART_recieveByte();
	while(1)
	{

		/*
		 * if init_flag equal the INIT_FLAG
		 * that mean the the first code isn't done yet
		 */
		if (init_flag != INIT_FLAG)
		{
			// Go to this function if the code is the first run to take the new password from user
			// 2 times and check if they are match and sent it to MCU22 to save it
			//
			takePassword();
			init_flag = INIT_FLAG;

			changePass();
		}
		else
		{
			// Go to this function if the code is done before run
			// to know if user want to change password or open the door
			//
			changePass();
		}
	}
	return 0;
}
