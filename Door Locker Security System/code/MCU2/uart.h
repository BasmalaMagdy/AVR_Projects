 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* UART Driver Baud Rate */
//#define USART_BAUDRATE 9600


/*******************************************************************************
 *                         	    STRUCTURE DECLARATION                              *
 *******************************************************************************/

typedef enum
{
	DISABLED,RESERVED,EVEN_PARITY,ODD_PARITY
}UART_Parity;

typedef enum
{
	BIT_1,BIT_2
}UART_StopBit;

typedef enum
{
	BIT_5,BIT_6,BIT_7,BIT_8,BIT_9
}UART_CharSize;

typedef struct
{
	UART_Parity parity;
	UART_StopBit stop;
	UART_CharSize size;
	uint32 s_baudrate; /*Take the required baudrate from the user*/
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(const UART_ConfigType * Config_Ptr);

void UART_sendByte(const uint8 data);

uint8 UART_recieveByte(void);

void UART_sendString(const uint8 *Str);

void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
