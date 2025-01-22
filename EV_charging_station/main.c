#include "FreeRTOS.h"
#include "task.h"
#include "LCD.h"

TaskHandle_t TaskChargingStartHandle, TaskStopSessionHandle, TaskDisplayRefreshHandle, TaskUserCommandHandle, TaskFaultDetectionHandle;

void TaskChargingStart(void *pvParameters);
void TaskStopSession(void *pvParameters);
void TaskDisplayRefresh(void *pvParameters);
void TaskUserCommand(void *pvParameters);
void TaskFaultDetection(void *pvParameters);

int main(void) {

	LCD_Init();
	LCD_String("ElectronicWINGS");
	LCD_Command(0xC0);      /* Go to 2nd line */
	LCD_String("Hello World");

	xTaskCreate(TaskChargingStart, "ChargingStart", 128, NULL, 3, &TaskChargingStartHandle);
	xTaskCreate(TaskStopSession, "StopSession", 128, NULL, 3, &TaskStopSessionHandle);
	xTaskCreate(TaskDisplayRefresh, "DisplayRefresh", 128, NULL, 2, &TaskDisplayRefreshHandle);
	xTaskCreate(TaskUserCommand, "UserCommand", 128, NULL, 2, &TaskUserCommandHandle);
	xTaskCreate(TaskFaultDetection, "FaultDetection", 128, NULL, 4, &TaskFaultDetectionHandle);

	vTaskStartScheduler();

	while (1) {
	}
}

// Charging Session Start Task (200ms)
void TaskChargingStart(void *pvParameters) {
	while (1) {

	}
}

// Stop Session Task (100ms)
void TaskStopSession(void *pvParameters) {
	while (1) {

	}
}

// Display Refresh Task (100ms)
void TaskDisplayRefresh(void *pvParameters) {
	while (1) {

	}
}

// User Command Response Task (100ms)
void TaskUserCommand(void *pvParameters) {
	while (1) {

	}
}

// Fault Detection Task (150ms)
void TaskFaultDetection(void *pvParameters) {
	while (1) {

	}
}
