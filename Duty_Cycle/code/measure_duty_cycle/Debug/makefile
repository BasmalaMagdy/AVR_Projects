################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
measure_duty_cycle.lss \

SIZEDUMMY += \
sizedummy \


# All Target
all: measure_duty_cycle.elf secondary-outputs

# Tool invocations
measure_duty_cycle.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,measure_duty_cycle.map -mmcu=atmega16 -o "measure_duty_cycle.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

measure_duty_cycle.lss: measure_duty_cycle.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S measure_duty_cycle.elf  >"measure_duty_cycle.lss"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: measure_duty_cycle.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega16 measure_duty_cycle.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) measure_duty_cycle.elf
	-@echo ' '

secondary-outputs: $(LSS) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
