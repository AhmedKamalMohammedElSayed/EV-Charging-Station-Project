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
	avr-gcc -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\FreeRtos" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\FreeRtos\include" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\MCAL" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\MCAL\ADC" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\OS" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\LOGIC" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\MCAL\Timer" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\HAL" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\Configrations" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\COMMUNICATION" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\HAL\Lcd" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\COMMUNICATION\Uart" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\MCAL\Gpio" -I"C:\Users\medo4\eclipse-workspace\EV_charging_station\Include\LOGIC\Polling" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32a -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


