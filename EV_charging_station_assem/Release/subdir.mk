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
	avr-gcc -I"D:\My Lectures\SBME\4th year (last one)\1st semester\Advanced Embedded Systems\Project\New folder\New folder\EV-Charging-Station-Project\EV_charging_station_assem\Include" -I"D:\My Lectures\SBME\4th year (last one)\1st semester\Advanced Embedded Systems\Project\New folder\New folder\EV-Charging-Station-Project\EV_charging_station_assem\Include\MCAL" -I"D:\My Lectures\SBME\4th year (last one)\1st semester\Advanced Embedded Systems\Project\New folder\New folder\EV-Charging-Station-Project\EV_charging_station_assem\Include\MCAL\ADC" -I"D:\My Lectures\SBME\4th year (last one)\1st semester\Advanced Embedded Systems\Project\New folder\New folder\EV-Charging-Station-Project\EV_charging_station_assem\Include\COMMUNICATION" -I"D:\My Lectures\SBME\4th year (last one)\1st semester\Advanced Embedded Systems\Project\New folder\New folder\EV-Charging-Station-Project\EV_charging_station_assem\Include\COMMUNICATION\Uart" -I"D:\My Lectures\SBME\4th year (last one)\1st semester\Advanced Embedded Systems\Project\New folder\New folder\EV-Charging-Station-Project\EV_charging_station_assem\Include\Configrations" -I"D:\My Lectures\SBME\4th year (last one)\1st semester\Advanced Embedded Systems\Project\New folder\New folder\EV-Charging-Station-Project\EV_charging_station_assem\Include\HAL\Lcd" -I"D:\My Lectures\SBME\4th year (last one)\1st semester\Advanced Embedded Systems\Project\New folder\New folder\EV-Charging-Station-Project\EV_charging_station_assem\Include\LOGIC" -I"D:\My Lectures\SBME\4th year (last one)\1st semester\Advanced Embedded Systems\Project\New folder\New folder\EV-Charging-Station-Project\EV_charging_station_assem\Include\LOGIC\Polling" -I"D:\My Lectures\SBME\4th year (last one)\1st semester\Advanced Embedded Systems\Project\New folder\New folder\EV-Charging-Station-Project\EV_charging_station_assem\Include\MCAL\Gpio" -I"D:\My Lectures\SBME\4th year (last one)\1st semester\Advanced Embedded Systems\Project\New folder\New folder\EV-Charging-Station-Project\EV_charging_station_assem\Include\MCAL\Timer" -I"D:\My Lectures\SBME\4th year (last one)\1st semester\Advanced Embedded Systems\Project\New folder\New folder\EV-Charging-Station-Project\EV_charging_station_assem\Include\OS" -I"D:\My Lectures\SBME\4th year (last one)\1st semester\Advanced Embedded Systems\Project\New folder\New folder\EV-Charging-Station-Project\EV_charging_station_assem\FreeRtos" -I"D:\My Lectures\SBME\4th year (last one)\1st semester\Advanced Embedded Systems\Project\New folder\New folder\EV-Charging-Station-Project\EV_charging_station_assem\FreeRtos\include" -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -lm -ffloat-store -mmcu=atmega32a -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


