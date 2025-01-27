################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRtos/croutine.c \
../FreeRtos/event_groups.c \
../FreeRtos/heap_1.c \
../FreeRtos/list.c \
../FreeRtos/port.c \
../FreeRtos/queue.c \
../FreeRtos/stream_buffer.c \
../FreeRtos/tasks.c \
../FreeRtos/timers.c 

OBJS += \
./FreeRtos/croutine.o \
./FreeRtos/event_groups.o \
./FreeRtos/heap_1.o \
./FreeRtos/list.o \
./FreeRtos/port.o \
./FreeRtos/queue.o \
./FreeRtos/stream_buffer.o \
./FreeRtos/tasks.o \
./FreeRtos/timers.o 

C_DEPS += \
./FreeRtos/croutine.d \
./FreeRtos/event_groups.d \
./FreeRtos/heap_1.d \
./FreeRtos/list.d \
./FreeRtos/port.d \
./FreeRtos/queue.d \
./FreeRtos/stream_buffer.d \
./FreeRtos/tasks.d \
./FreeRtos/timers.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRtos/%.o: ../FreeRtos/%.c FreeRtos/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\MCAL" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\MCAL\ADC" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\COMMUNICATION" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\COMMUNICATION\Uart" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\Configrations" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\HAL\Lcd" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\LOGIC" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\LOGIC\Polling" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\MCAL\Gpio" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\MCAL\Timer" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\OS" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\FreeRtos" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\FreeRtos\include" -I"C:\Users\Horizon\Documents\GitHub\EV-Charging-Station-Project\EV_charging_station_assem\Include\LOGIC\observer" -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -lm -ffloat-store -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


