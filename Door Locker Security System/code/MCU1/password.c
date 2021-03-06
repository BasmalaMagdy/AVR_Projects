/********************************************************************************
 * File Name: password.c
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
#include "password.h"
#include "timer.h"

/*******************************************************************************
 *                                  Definitions                                *
 *******************************************************************************/
#define M1_READY 0x01 /*MCU1 send to MCU2 when it is ready*/
#define M2_READY 0x02 /*MCU2 send to MCU1 when it is ready*/
#define Begin 0x03 /*MCU1 make MCU2 to begin saving the password*/
#define End 0x04 /*MCU2 send to MCU1 to inform that saving password has been finished*/
#define compPass 0x05 /*MCU1 send to MCU2 to start comparing passwords*/
#define wrong 0x06 /*MCU2 send to MCU1 that entered password was wrong*/
#define correct 0x07 /*MCU2 send to MCU1 that entered password was correct*/
#define M2_comREADY 0x08 /*MC2 send to MC1 when it is ready in compare*/
#define Open 0x09 /*MC2 send to MC1 when it is ready to open door*/
#define INIT_FLAG 0x0A /*Flag is saved if the one run code is done*/

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
volatile uint8 g_errCount =  0;          /*variable used to count how many user enter password wrong*/
volatile uint8 g_error_mesg = 0;          /*variable used to know if user enter password wrong 3 times*/
volatile uint8 g_error_flag = 0;
volatile uint8 g_timeCount = 0;            /*variable used to count time*/
volatile uint8 g_sec_Count = 0;            /*variable used to count number of seconds*/
/* FUNCTION to get password from user and check 2 password and save it*/
void takePassword(void)
{
	/*******************************************************************************
	 *                          LOCAL VARIABLE                                  *
	 *******************************************************************************/
	uint8 a_key = 0;                                        /*variable used to receive password*/
	uint8 a_count;                                          /*variable used to count received password*/
	uint8 a_Pass[6]  , a_rePass[6];                         /*variable used to save password*/
	uint8 a_strPass[6], a_str_rePass[6];                    /*variable used to save password as string*/
	uint8 a_strCompare = 0;                                 /*variable used to know if 2enter password are match or not*/


	LCD_clearScreen();           /*clear lcd*/
	LCD_displayStringRowColumn(0,0,"Enter new pass:");
	/*loop to get password from user*/
	for(a_count = 0; a_count < 5; a_count++)
	{
		 /*get password*/
		a_key = KeyPad_getPressedKey();
		/*wait until user press enter*/
		while(!((a_key >= 0) && (a_key <= 9)))
		{
			a_key = KeyPad_getPressedKey();
		}

		/*LCD_goToRowColumn(1 , a_count);
		LCD_intgerToString(a_key);*/
		LCD_displayStringRowColumn(1,a_count + 3,"*");

		a_Pass[a_count] = a_key;
		itoa(a_Pass[a_count], a_strPass, 10);
		_delay_ms(500);
	}
	while (KeyPad_getPressedKey() != 13){}


	LCD_clearScreen();         /*clear lcd*/
	/*ask user reenter new password*/
	LCD_displayStringRowColumn(0,0,"ReEnter pass:");
	/*loop to get password from user*/
	for(a_count = 0; a_count < 5; a_count++)
	{
		 /*get password*/
		a_key = KeyPad_getPressedKey();
		/*wait until user press number from 0 to 9*/
		while(!((a_key >= 0) && (a_key <= 9)))
		{
			a_key = KeyPad_getPressedKey();
		}

		/*LCD_goToRowColumn(1 , a_count);
		LCD_intgerToString(a_key);*/
		/*ask user reenter new password*/
		LCD_displayStringRowColumn(1,a_count + 3,"*");

		a_rePass[a_count] = a_key;                            /*save password in array*/
		itoa(a_rePass[a_count], a_str_rePass, 10);            /*convert password to string*/
		_delay_ms(500);
	}
	while (KeyPad_getPressedKey() != 13){}     /*wait until user press enter*/

	a_strCompare = strcmp(a_strPass, a_str_rePass);    /*check password*/
	/*if 2 password are match*/
	if (a_strCompare == 0)
	{
		LCD_clearScreen();          /*clear lcd*/
		LCD_displayStringRowColumn(0, 3, "password");
		LCD_displayStringRowColumn(1, 4, "Match");
		_delay_ms(200);


		UART_sendByte(Begin);
		for(a_count = 0; a_count < 5; a_count++)
		{
			/*MCU1 send to MCU2 to start take password*/
			while(UART_recieveByte()!= M2_READY){};
			/*MCU1 Send password to MCU2*/
			UART_sendByte(a_Pass[a_count]);
		}
		/*MCU1 wait until MCU2 be end*/
		while(UART_recieveByte()!= End);

	}
	else
	{
		LCD_clearScreen();           /*clear lcd*/
		LCD_displayStringRowColumn(0, 0, "pass NOT MATCH");
		LCD_displayStringRowColumn(1, 0, "Please ReEnter");
		_delay_ms(300);
		/*if password not match go back to tack password again from user*/
		takePassword();
	}
}

/* FUNCTION to ask user if he want to chande password or open the door*/
void changePass(void)
{
	/*******************************************************************************
		 *                          LOCAL VARIABLE                                  *
		 *******************************************************************************/
	uint8 a_key = 0;                   /*variable used to receive password*/

	LCD_clearScreen();             /*clear lcd*/
	/*ask user if he want to change password or open door*/
	LCD_displayStringRowColumn(0, 0, "+ : Change Pass");
	LCD_displayStringRowColumn(1, 0, "- :Open the Door");
	/*save enter password in key*/
	a_key = KeyPad_getPressedKey();
	/*wait until user enter + or -*/
	while (!(a_key == '+' || a_key == '-'))
	{
		a_key = KeyPad_getPressedKey();
	}
	/*send password to compare function to compare it with saved password*/
	comparePass();
	/*send number of error to MCU2*/
	UART_sendByte(g_error_flag);
	/*if received error counter = 0 */
	if(g_error_flag == 0)
	{
		/*MCU1 send + or - to MCU2 */
		UART_sendByte(a_key);

		if(a_key == '+')
		{
			 /*IF user enter + that mean go to take new password*/
			takePassword();
		}
		else if(a_key == '-')
		{
			/*IF user enter - that mean go to open door*/
			OpenDoor();
		}
	}
	else
	{
		/*IF error counter not equal 0 */
		g_error_flag = 0;
	}
}

/* FUNCTION to compare 2 password one that user entered and anther that we saved before */
void comparePass(void)
{
	/*******************************************************************************
		 *                          LOCAL VARIABLE                                  *
		 *******************************************************************************/
	uint8 a_key = 0;            /*variable used to receive password*/
	uint8 a_count = 0;          /*variable used to count received password*/
	/*uint8 a_errdigit = 0;*/
	uint8 a_Pass[6];            /*variable used to save received password*/

	LCD_clearScreen();          /*clear lcd*/
	LCD_displayStringRowColumn(0,0,"Enter pass:");
	/*loop to get password from user*/
	for(a_count = 0; a_count < 5; a_count++)
	{
		/*get password*/
		a_key = KeyPad_getPressedKey();
		/*wait until user press enter*/
		while(!((a_key >= 0) && (a_key <= 9)))
		{
			a_key = KeyPad_getPressedKey();
		}

		/*LCD_goToRowColumn(1 , a_count);
		LCD_intgerToString(a_key);*/
		LCD_displayStringRowColumn(1,a_count + 3,"*");
		/*saved enterd password in array*/
		a_Pass[a_count] = a_key;

		_delay_ms(500);
	}
	/*MCU1 wait until MCU2 be end*/
	while (KeyPad_getPressedKey() != 13){}
	/*MCU1 wait until send to MCU2 be compPass*/
	UART_sendByte(compPass);
	/*loop to send password to MCU2*/
	for(a_count = 0; a_count < 5; a_count++)
	{
		/*MCU1 wait until receive M2_comREADY from MCU2 to be ready*/
		while(UART_recieveByte()!= M2_comREADY){};
		/*send password byte by byte to MCU2*/
		UART_sendByte(a_Pass[a_count]);
	}
	 /* if MCU1 receive wrong from MCU2*/
	if(UART_recieveByte() == wrong)
	{
		/*increase error counter by one*/
		g_errCount ++;
		/*MCU1 send error counter to MCU2*/
		UART_sendByte(g_errCount);
		/*if error counter less than 3*/
		if(g_errCount < 3)
		{
			/*print wrong password on lcd*/
			LCD_clearScreen();         /*clear lcd*/
			LCD_displayStringRowColumn(0,3,"Wrong");
			LCD_displayStringRowColumn(1,3,"Password");
			_delay_ms(500);
			/*return to compare function to take anther password*/
			comparePass();
		}
		else
		{
			LCD_clearScreen();    /*clear lcd*/

			g_timeCount = 0;
			g_sec_Count = 0;
			Timer_start();
			while (g_sec_Count != 60) /*As long as it hasn't passed 1 minute,display on LCD this meassage*/
			{
				LCD_displayStringRowColumn(0,4,"ERROR");
			}
			Timer_stop();
			g_errCount = 0;
			g_error_flag = 1;      /*set flag as 1*/

		}
	}
	else
	{
		UART_sendByte(0);
		g_errCount = 0;
		g_error_flag = 0;
	}
}
/*FUNCTION used to open door*/
void OpenDoor (void)
{
	UART_sendByte(Open);
	 /*make motor move as clock wise to open door*/
	g_timeCount = 0;
	g_sec_Count = 0;
	Timer_start();
	LCD_clearScreen(); /*Clear LCD*/
	LCD_displayStringRowColumn(0,4,"OPEN DOOR");
	while(g_sec_Count != 3)
	{

	}
	Timer_stop();
	/*make motor move as anti clock wise to close door*/
	g_timeCount = 0;
	g_sec_Count = 0;
	Timer_start();
	LCD_clearScreen(); /*Clear LCD*/
	LCD_displayStringRowColumn(0,4,"CLOSE DOOR");
	while(g_sec_Count != 3)
	{

	}
	Timer_stop();


}
/*FUNCTION to calculate number of seconds*/
void Calc_time(void)
{
	g_timeCount++;   /*increase time counter by one*/

	if(g_timeCount == 30)
	{
		g_timeCount = 0;
		g_sec_Count ++;            /*increase seconds counter by one*/
	}
}

