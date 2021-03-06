 /******************************************************************************
 *
 * Module: ADC
 *
 * File Name: adc.c
 *
 * Description: Source file for the ADC driver
 *
 * Author: Basmala Magdy
 *
 *******************************************************************************/

#include "adc.h"

/*******************************************************************************
 *                          Global Variables                                   *
 *******************************************************************************/

volatile uint16 g_adcResult = 0;

/*******************************************************************************
 *                          ISR's Definitions                                  *
 *******************************************************************************/

ISR(ADC_vect)
{
	/* Read ADC Data after conversion complete */
	g_adcResult = ADC;
}

void ADC_init(void)
{
	ADMUX = 0;
	ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADPS1) | (1<<ADPS0);
}

void ADC_readChannel(uint8 channel_num)
{
	ADMUX = (ADMUX & 0xE0) | (channel_num & 0X07);
	SET_BIT(ADCSRA,ADSC);

}
