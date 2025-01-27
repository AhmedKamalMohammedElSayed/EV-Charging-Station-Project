################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Include/MCAL/ADC/ADC.c 

OBJS += \
./Include/MCAL/ADC/ADC.o 

C_DEPS += \
./Include/MCAL/ADC/ADC.d 


# Each subdirectory must supply rules for building sources it contributes
Include/MCAL/ADC/%.o: ../Include/MCAL/ADC/%.c Include/MCAL/ADC/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\MCAL" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\MCAL\ADC" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\COMMUNICATION" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\COMMUNICATION\Uart" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\Configrations" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\HAL\Lcd" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\LOGIC" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\LOGIC\Polling" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\MCAL\Gpio" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\MCAL\Timer" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\OS" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\FreeRtos" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\FreeRtos\include" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\LOGIC\observer" -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -lm -ffloat-store -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


