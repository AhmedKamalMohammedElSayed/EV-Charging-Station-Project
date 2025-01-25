################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Include/OS/subdir.mk
-include Include/MCAL/Timer/subdir.mk
-include Include/MCAL/Gpio/subdir.mk
-include Include/MCAL/ADC/subdir.mk
-include Include/LOGIC/Polling/subdir.mk
-include Include/LOGIC/subdir.mk
-include Include/HAL/Lcd/subdir.mk
-include Include/COMMUNICATION/Uart/subdir.mk
-include FreeRtos/subdir.mk
-include Debug/Include/OS/subdir.mk
-include Debug/Include/MCAL/Timer/subdir.mk
-include Debug/Include/MCAL/Gpio/subdir.mk
-include Debug/Include/MCAL/Adc/subdir.mk
-include Debug/Include/Lcd/subdir.mk
-include Debug/Include/LOGIC/Polling/subdir.mk
-include Debug/Include/LOGIC/subdir.mk
-include Debug/Include/HAL/Lcd/subdir.mk
-include Debug/Include/COMMUNICATION/Uart/subdir.mk
-include Debug/FreeRtos/subdir.mk
-include Debug/subdir.mk
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

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := EV_charging_station
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
EV_charging_station.lss \

FLASH_IMAGE += \
EV_charging_station.hex \

EEPROM_IMAGE += \
EV_charging_station.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: main-build

# Main-build Target
main-build: EV_charging_station.elf secondary-outputs

# Tool invocations
EV_charging_station.elf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,EV_charging_station.map -mmcu=atmega32a -o "EV_charging_station.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

EV_charging_station.lss: EV_charging_station.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S EV_charging_station.elf  >"EV_charging_station.lss"
	@echo 'Finished building: $@'
	@echo ' '

EV_charging_station.hex: EV_charging_station.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex EV_charging_station.elf  "EV_charging_station.hex"
	@echo 'Finished building: $@'
	@echo ' '

EV_charging_station.eep: EV_charging_station.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex EV_charging_station.elf  "EV_charging_station.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: EV_charging_station.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32a EV_charging_station.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) EV_charging_station.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents main-build

-include ../makefile.targets
