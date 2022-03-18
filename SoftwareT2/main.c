
/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "io.h"
#pragma config POSCMOD = HS
#pragma config FNOSC = PRI
static void system_boot(void*);
int main(void)
{
	xTaskCreate(system_boot, "Check", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	vTaskStartScheduler();
    while(1);
	return 0;
}
static void system_boot(void* ptr)
{
    hardware_init();
    lcd_write_string("Iniciando...");
    while(1);
}