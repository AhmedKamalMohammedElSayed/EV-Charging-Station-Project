################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Include/LOGIC/Logic.c 

OBJS += \
./Include/LOGIC/Logic.o 

C_DEPS += \
./Include/LOGIC/Logic.d 


# Each subdirectory must supply rules for building sources it contributes
Include/LOGIC/%.o: ../Include/LOGIC/%.c Include/LOGIC/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\FreeRtos" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\FreeRtos\include" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\MCAL" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\MCAL\ADC" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\OS" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\LOGIC" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\MCAL\Timer" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\HAL" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\Configrations" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\COMMUNICATION" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\HAL\Lcd" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\COMMUNICATION\Uart" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\MCAL\Gpio" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\LOGIC\Polling" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32a -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


