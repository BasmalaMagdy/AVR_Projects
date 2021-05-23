 /******************************************************************************
 *
 * Module: Micro - Configuration
 *
 * File Name: Micro_Config.h
 *
 * Description: File include all Microcontroller libraries
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef MICRO_CONFIG_H_
#define MICRO_CONFIG_H_

#undef F_CPU
#ifndef F_CPU
#define F_CPU 8000000UL //8MHz Clock frequency
#endif  

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include "std_types.h"
#include "common_macros.h"
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "password.h"
#include "i2c.h"
#include "timer.h"

#endif /* MICRO_CONFIG_H_ */
