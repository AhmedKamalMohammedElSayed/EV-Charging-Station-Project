################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main.c 

OBJS += \
./main.o 

C_DEPS += \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station\FreeRtos" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station\FreeRtos\include" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station\Include" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station\Include\Lcd" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station\Include\MCAL" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station\Include\MCAL\ADC" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32a -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


