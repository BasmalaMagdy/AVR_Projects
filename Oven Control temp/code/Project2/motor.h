 /******************************************************************************
 *
 * File Name: motor.h
 *
 * Created on: Feb 15, 2021
 *
 * Author: Basmala Magdy
 *
 *******************************************************************************/

#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"


/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
/* Motor HW Pins */
#define IN1 PB0
#define IN2 PB1
#define EN1 PB3
#define INT PD3

#define MOTOR_OUT_PORT PORTB
#define MOTOR_OUT_PORT_DIR DDRB
#define MOTOR_IN_PORT PORTD
#define MOTOR_IN_PORT_DIR DDRD

/* MOTOR Register */
#define Timer0_Initial_value TCNT0
#define Compare_Value OCR0
#define Control_Value TCCR0

/* MOTOR Register */
#define INT1_MCU_Control      MCUCR
#define General_INT1_Control   GICR


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void PWM_Timer0_Init(unsigned char set_duty_cycle);
void INT1_reverse(void);



#endif /* MOTOR_H_ */




