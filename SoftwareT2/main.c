
/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
static void t_ex(void);
int main(void)
{
	xTaskCreate(t_ex, "Check", 64, NULL, 2, NULL);
	vTaskStartScheduler();
	return 0;
}
static void t_ex()
{
    while(1);
    
}