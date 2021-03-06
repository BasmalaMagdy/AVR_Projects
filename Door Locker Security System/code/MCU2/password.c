/********************************************************************************
 * File Name: password.c
 *
 * Author: Basmala Magdy
 *
 * Created on: Mar 30, 2021
 *
 * Description: function used in main
 *******************************************************************************/
/*******************************************************************************
 *                                   INCLUDES                                  *
 *******************************************************************************/
#define F_CPU 8000000UL
#include "password.h"
#include "external_eeprom.h"
#include "timer.h"
/*******************************************************************************
 *                                  Definitions                                *
 *******************************************************************************/
#define M1_READY 0x01 /*MC2 send to MC1 when it is ready*/
#define M2_READY 0x02 /*MC2 send to MC1 when it is ready*/
#define Begin 0x03 /*MCU1 make MCU2 to begin saving the password*/
#define End 0x04 /*MC2 send to MC1 to inform that saving password has been finished*/
#define compPass 0x05 /*MC2 send to MC1 to start comparing passwords*/
#define wrong 0x06 /*MC2 send to MC1 that entered password was wrong*/
#define correct 0x07 /*MC2 send to MC1 that entered password was correct*/
#define M2_comREADY 0x08 /*MC2 send to MC1 when it is ready in compare*/
#define Open 0x09 /*MC2 send to MC1 when it is ready to open door*/
#define INIT_FLAG 0x0A /*Flag is saved if the one run code is done*/
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
volatile uint8 g_flag = 0;             /*flag to know if user enter wrong password*/
volatile uint8 g_errCount =  0;        /*variable used to count how many user enter password wrong*/
volatile uint8 g_error_flag = 0;       /*variable used to know if user enter password wrong 3 times*/
volatile uint8 g_timeCount = 0;        /*variable used to count time*/
volatile uint8 g_sec_Count = 0;        /*variable used to count number of seconds*/

/* FUNCTION to get password from user and check 2 password and save it*/
void recievePassword(void)
{
	/*******************************************************************************
	 *                          LOCAL VARIABLE                                  *
	 *******************************************************************************/
	uint8 a_Pass[6];                    /*variable used to save password*/
	/*uint8 a_rPass[6];*/
	uint8 a_count;                      /*variable used to count received password*/

	/*MCU2 wait until MCU1 send to start*/
	while(UART_recieveByte()!= Begin);
	/*LCD_clearScreen();
	LCD_displayString("ccccc");*/
	/*loop to save password*/
	for(a_count = 0; a_count < 5; a_count++)
	{
		/*MCU2 send to MCU1 that it be ready*/
		UART_sendByte(M2_READY);
		/*save received password from MCU1 in array*/
		a_Pass[a_count] = UART_recieveByte();
		/*LCD_goToRowColumn(1 , a_count);
		LCD_intgerToString(a_Pass[a_count]);*/
		/*save received password from MCU1 in memory*/
		EEPROM_writeByte((0x0311+a_count), a_Pass[a_count]);

		/*_delay_ms(10);
		EEPROM_readByte((0x0311+a_count), &a_rPass[a_count]);*/


		//LCD_displayStringRowColumn(1,a_count + 3,a_strPass[a_count]);
		_delay_ms(10);                /*Wait for 10ms to write the value in the memory*/
	}
	UART_sendByte(End);
	/*save flag in memory address to know if code used one before or nor*/
	EEPROM_writeByte(0x0420, INIT_FLAG);
	_delay_ms(10);/*Wait for 10ms to write the value in the memory*/
}
/* FUNCTION to ask user if he want to chande password or open the door*/
void changePass(void)
{
	uint8 a_key;
	/*call compare password to check if entered password is correct or not*/
	comparePass();
	/*if error counter equal 0 */
	g_error_flag = UART_recieveByte();

	if(g_error_flag == 0)
	{
		/*LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"IN");*/

		a_key = UART_recieveByte();
		/*LCD_intgerToString(a_key);*/
		/*if received key is + go to receive new password*/
		if(a_key == 13)
		{
			/*LCD_displayStringRowColumn(0,3,"open");*/
			/*if received key is - go to open door*/
			OpenDoor();

		}
		else if (a_key == 11)
		{
			/*if received key is + go to receive new password*/
			recievePassword();
		}
		/*LCD_displayStringRowColumn(1,0,"out");*/
	}
	else
	{
		/*close led*/
		CLEAR_BIT(PORTD,PD2);
	}
}
/* FUNCTION used to compare 2 password one that user entered and anther that we saved before */
void comparePass(void)
{
	/*******************************************************************************
	 *                          LOCAL VARIABLE                                  *
	 *******************************************************************************/
	uint8 a_savedPass[6];           /*array has saved password in memory*/
	uint8 a_Pass[6];                /*array has saved password from user*/
	uint8 a_count = 0;               /*variable used to count received password*/
	uint8 a_errCount = 0;           /*variable used to count how many user enter password wrong*/
	/*MCU2 wait until MCU1 send to compPass*/
	while(UART_recieveByte()!= compPass);
	/*loop to receive password from user*/
	for(a_count = 0; a_count < 5; a_count++)
	{
		/*MCU2 send to MCU1 that it be ready*/
		UART_sendByte(M2_comREADY);
		/*saved received password in array*/
		a_Pass[a_count] = UART_recieveByte();
		/*LCD_goToRowColumn(1 , a_count);
		LCD_intgerToString(a_Pass[a_count]);*/
	}
	/*loop to get saved password from memory password from user*/
	for(a_count = 0; a_count < 5; a_count++)
	{
		EEPROM_readByte((0x0311+a_count), &a_savedPass[a_count]);
	}
	/*loop to compare passwords*/
	for(a_count = 0; a_count < 5; a_count++)
	{
		if(a_Pass[a_count] != a_savedPass[a_count])
		{
			/*if any number in password was wrong put 1 in flag*/
			g_flag = 1;

			/*LCD_clearScreen();
			LCD_displayStringRowColumn(0,3,"wrong");*/
			break;
		}
	}
	if(g_flag == 1)
	{
		/*MCU2 send wrong to MCU1 in case password was wrong*/
		UART_sendByte(wrong);
		g_flag = 0;
	}
	else
	{
		/*MCU2 send correct to MCU1 in case password was correct*/
		UART_sendByte(correct);
	}
	/*MCU2 wait until MCU1 send to error counter*/
	a_errCount = UART_recieveByte();
	g_errCount = a_errCount;

	if(a_errCount < 3 && a_errCount>0)
	{
		/*if error counter not equal 3 and 0 go to compare function to take password again*/
		comparePass();
	}
	else if (a_errCount == 3)
	{
		/*if error counter equal 3 open led*/
		g_timeCount = 0;
		g_sec_Count = 0;
		Timer_start();
		SET_BIT(PORTD,PD2);
		while (g_sec_Count != 60) /*As long as it hasn't passed 1 minute,display on LCD this massage*/
		{

		}
		Timer_stop();
	}
}
/*FUNCTION used to open door*/
void OpenDoor (void)
{
	/*wait until MCU1 send Open to MCU2*/
	while(UART_recieveByte()!= Open);
	/*make motor move as clock wise to open door*/
	g_timeCount = 0;
	g_sec_Count = 0;
	Timer_start();
	PORTC &= (~(1<<PC2));
	PORTC |= (1<<PC3);
	while(g_sec_Count != 3)
	{

	}
	Timer_stop();
	/*make motor move as anti clock wise to close door*/
	g_timeCount = 0;
	g_sec_Count = 0;
	Timer_start();
	PORTC &= (~(1<<PC3));
	PORTC |= (1<<PC2);
	while(g_sec_Count != 3)
	{

	}
	Timer_stop();

	//Then stop the Door
	CLEAR_BIT(PORTC,PC2);
	CLEAR_BIT(PORTC,PC3);

}
/*FUNCTION to calculate number of seconds*/
void Calc_time(void)
{
	/*increase time counter by one*/
	g_timeCount++;

	if(g_timeCount == 30)
	{
		g_timeCount = 0;
		/*increase seconds counter by one*/
		g_sec_Count ++;
	}
}


