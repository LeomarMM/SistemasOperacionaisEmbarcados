
/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#pragma config POSCMOD = HS
#pragma config FNOSC = PRI
void system_boot(void* ptr);
int main(void)
{
	xTaskCreate(system_boot, "sysboot", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	vTaskStartScheduler();
    while(1);
	return 0;
}