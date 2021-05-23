################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MCU22.c \
../adc.c \
../external_eeprom.c \
../i2c.c \
../keypad.c \
../lcd.c \
../password.c \
../timer.c \
../uart.c 

OBJS += \
./MCU22.o \
./adc.o \
./external_eeprom.o \
./i2c.o \
./keypad.o \
./lcd.o \
./password.o \
./timer.o \
./uart.o 

C_DEPS += \
./MCU22.d \
./adc.d \
./external_eeprom.d \
./i2c.d \
./keypad.d \
./lcd.d \
./password.d \
./timer.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


