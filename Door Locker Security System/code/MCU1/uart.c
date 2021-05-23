 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#include "uart.h"

//#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 8UL))) - 1)

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void UART_init(const UART_ConfigType * Config_Ptr)
{
	uint32 baudrate; /*Used to put baudrate in it*/

	uint32 baud_prescaler;

	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);
	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/ 
	UCSRB = (1<<RXEN) | (1<<TXEN);
	

	if (Config_Ptr-> parity == EVEN_PARITY)
	{
		SET_BIT(UCSRC,UPM1);
		//UCSRC |= (1<<UPM1); /*UPM1=1: Even parity*/
	}
	else if(Config_Ptr-> parity == ODD_PARITY)
	{
		SET_BIT(UCSRC,UPM1);
		SET_BIT(UCSRC,UPM0);
		//UCSRC |= (1<<UPM0) | (1<<UPM1); /*UPM1=0 UPM1=1: Odd parity*/
	}


	if(Config_Ptr->stop == BIT_2)
	{
		SET_BIT(UCSRC,USBS);
		//UCSRC |= (1<<USBS); /*USBS=1: 2-bits stop*/
	}

	if(Config_Ptr->size == BIT_6)
	{
		SET_BIT(UCSRC,UCSZ0);
		//UCSRC |= (1<<UCSZ0);
	}
	else if(Config_Ptr->size == BIT_7)
	{
		SET_BIT(UCSRC,UCSZ1);
		//UCSRC |= (1<<UCSZ1);
	}
	else if(Config_Ptr->size == BIT_8)
	{
		SET_BIT(UCSRC,UCSZ1);
		SET_BIT(UCSRC,UCSZ0);
		//UCSRC = (1<<UCSZ0) | (1<<UCSZ1);  /*For 8-bit data mode*/
	}

	UCSRC = (1<<URSEL);
	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  = 00 Disable parity bit
	 * USBS    = 0 One stop bit
	 * UCSZ1:0 = 11 For 8-bit data mode
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/ 	
	


	baudrate =  Config_Ptr->s_baudrate;

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	baud_prescaler = (((F_CPU / (baudrate * 8UL))) - 1);
	UBRRH = baud_prescaler>>8;
	UBRRL = baud_prescaler;
}
	
void UART_sendByte(const uint8 data)
{
	/* UDRE flag is set when the Tx buffer (UDR) is empty and ready for 
	 * transmitting a new byte so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}
	/* Put the required data in the UDR register and it also clear the UDRE flag as 
	 * the UDR register is not empty now */	 
	UDR = data;
	/************************* Another Method *************************
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transimission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	*******************************************************************/	
}

uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this 
	 * flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}
	/* Read the received data from the Rx buffer (UDR) and the RXC flag 
	   will be cleared after read this data */	 
    return UDR;		
}

void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
	/************************* Another Method *************************
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}		
	*******************************************************************/
}

void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;
	Str[i] = UART_recieveByte();
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}
	Str[i] = '\0';
}
