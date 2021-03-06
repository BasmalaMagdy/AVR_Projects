/*
 * 7seg.c
 *
 *  Created on: Feb 3, 2021
 *      Author: Basmala Magdy
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
/*----------------------- Variables ----------------------------- */
unsigned char sec_num = 0;
unsigned char min_num = 0;
unsigned char hr_num = 0;
unsigned char INT_Flag = 0;
unsigned char stop = 1;          /*flag for stop & resume*/

/*----------------------- Functions ----------------------------- */
void INT0_RESET(void)
{
	PORTD |= (1 << PD2);                    /* activate input pull up*/
	MCUCR |= (1 << ISC01);                  /* falling edge*/
	GICR |= (1 << 6);                       /* GENERATE INT0*/

}
ISR(INT0_vect)
{
	sec_num = 0;
	min_num = 0;
	hr_num = 0;
}
/* -------------------------------------------------------------*/
void INT1_PAUSE(void)
{
	MCUCR |= (1 << ISC01) | (1 << ISC11);                   /* Raising edge*/
	GICR |= (1 << 7);                                       /* GENERATE INT1*/
}
ISR(INT1_vect)
{
	if(stop == 1)
	{
		TCCR1B &= ~ (1<<WGM12) & (1<<CS12) & (1<<CS10);     /*No clock source(Timer/Counter stopped)*/
		stop = 0;
	}
	else{}
}

/* -------------------------------------------------------------*/
void INT2_RESUME(void)
{
	PORTB |= (1 << PB2);                   /* activate input pull up*/
	MCUCSR &=~ (1 << ISC2);                /* Falling edge*/
	GICR |= (1 << 5);                     /* GENERATE INT2*/
}
ISR(INT2_vect)
{
	if(stop == 0)
	{
		TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);
		stop = 1;
	}
	else{}

}
/* -------------------------------------------------------------*/
void Timer1_init(void)
{
	TCNT1 = 0;
	OCR1A = 976;                                                /* F=1M/1024,T=1024/1M=1.024ms,clocks=1/1.024ms = 976 ,in1000ms*/
	TIMSK |= (1<<OCIE1A);                                       /*Enable Timer1 Compare A*/
	TCCR1A = (1<<FOC1A) ;                                       /* non-PWM*/
	TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);                /* prescaler = 1024 , MODE = 4 , CTC */
}
ISR(TIMER1_COMPA_vect)
{
	INT_Flag = 1;
}

/* -------------------------------------------------------------*/

int main()
{

	DDRB &= (1 << PB2);                                                 /* configure PB2 as input pin */
	DDRD &= (1 << PD2);                                                 /*configure PD2 as input pin*/
	DDRD &= (1 << PD3);                                                 /*configure PD3 as input pin*/
	DDRC |= (1<<PC0) | (1<<PC1) | (1<<PC2) | (1<<PC3);                  /*configure PC0,1,2,3 as output pins*/

	/* Initialize the 7-segment to ZERO */
	PORTC &= 0x00;

	/*----------- ENABLE -------------------*/
	DDRA |= 0x3F;                                            /* configure PA0,1,2,3,4,5 as output pin */
	PORTA |= 0x3F;                                           /* PA0,1,2,3,4,5 = 1 (NPN transistor Enable = 1) */

	SREG |=(1<<7) ;

	INT0_RESET();
	INT1_PAUSE();
	INT2_RESUME();
	Timer1_init();

	while(1)
	{
		if(INT_Flag == 1)
		{
			sec_num++;
			/*----- count seconds -----*/
			if(sec_num == 60)
			{
				sec_num = 0;
				min_num++;
			}
			/*----- count minutes -----*/
			if(min_num == 60)
			{
				min_num = 0;
				hr_num++;
			}
			/*----- count Hours -----*/
			if(hr_num == 24)
			{
				hr_num = 0;
				min_num = 0;
				sec_num = 0;
			}
			INT_Flag = 0;
		}
		else{
			/*------------------- display seconds --------------------*/
			PORTA = (1<<PA0);
			PORTC = (PORTC & 0xF0) | ((sec_num % 10) & 0x0F);
			_delay_ms(2);
			PORTA = (1<<PA1);
			PORTC = (PORTC & 0xF0) | ((sec_num / 10) & 0x0F);
			_delay_ms(2);
			/*------------------- display minutes --------------------*/
			PORTA = (1<<PA2);
			PORTC = (PORTC & 0xF0) | ((min_num % 10) & 0x0F);
			_delay_ms(2);
			PORTA = (1<<PA3);
			PORTC = (PORTC & 0xF0) | ((min_num / 10) & 0x0F);
			_delay_ms(2);
			/*------------------- display hours --------------------*/
			PORTA = (1<<PA4);
			PORTC = (PORTC & 0xF0) | ((hr_num % 10) & 0x0F);
			_delay_ms(2);
			PORTA = (1<<PA5);
			PORTC = (PORTC & 0xF0) | ((hr_num / 10) & 0x0F);
			_delay_ms(2);
		}
	}
}



