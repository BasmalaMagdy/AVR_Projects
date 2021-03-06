/********************************************************************************
 * File Name: MCU22.c
 *
 * Author: Basmala Magdy
 *
 * Created on: Mar 30, 2021
 *
 * Description: Main file of MCU22 for Final Project
 *******************************************************************************/
/*******************************************************************************
 *                                   INCLUDES                                  *
 *******************************************************************************/

#define F_CPU 8000000UL
#include "micro_config.h"

/*******************************************************************************
 *                                  Definitions                                *
 *******************************************************************************/
#define M1_READY 0x01 /*MC2 send to MC1 when it is ready*/
#define INIT_FLAG 0x0A /*Flag is saved if the one run code is done*/

/*******************************************************************************
 *                                 MAIN FUNCTION                              *
 *******************************************************************************/

int main(void)
{
	uint8 init_flag =0;      /*variable used to know if the first code is done before or not*/

	DDRC |= 0X0C;
	PORTC &= 0XF3;

	DDRD |= 0X04;
    PORTD &= 0XF3;

	LCD_init();    /* initialize LCD */

	/*
	 * We pass the input to the structure of I2C
	 * 1. Prescalar: Fcpu
	 * 2. bit_rate_regester(TWBR): 2 (according to the function Fscl=(Fcpu)/((16)+(2*TWBR*pow(4,TWPS)))
	 * 5. Slave address: 0
	 */
	TWI_ConfigType TWI_Config = {F_CPU_CLOCK,0x02,0x00,0};
	EEPROM_init(& TWI_Config);
	/*
	 *  the structure of timer
	 * 1. Timer_type: Timer0
	 * 2. Timer_mode: Overflow mode
	 * 3. Prescalar: clk/1024
	 * 4. Initial value: 0
	 * 5. Compare value: 0  (overflow mode)
	 */
	Timer_ConfigType Timer_Config = {TIMER0,OVERFLOW,F_CPUt_1024,0,0};
	/*
	 * the structure of UART
	 * 1. Parity: Disabled (no parity)
	 * 2. Stop_Bit: BIT_1 (1 stop bit)
	 * 3. CharSize: BIT_8 (8-bits data)
	 * 4. Baudrate: 9600
	 */
	UART_ConfigType UART_Config = {DISABLED,BIT_1,BIT_8,9600};

	UART_init( & UART_Config);     /* initialize UART */

	Timer_init(& Timer_Config);   /* initialize TIMER */

	Timer0_Ovf_setCallBack(Calc_time);


	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);
	while(UART_recieveByte()!= M1_READY);  /*MC1 sends to MC2 that it is ready to start*/

	EEPROM_readByte(0x0420, &init_flag);

	UART_sendByte(init_flag);
	while(1)
	{
		if (init_flag != INIT_FLAG)
		{
			recievePassword();
			init_flag = INIT_FLAG;
			changePass();
		}
		else
		{
			changePass();
		}
	}
return 0;
}
