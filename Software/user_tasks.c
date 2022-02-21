#include "user_tasks.h"
#include "semaphore.h"
#include "io.h"
#include "kernel.h"
#include <xc.h>
void config_tasks()
{
    asm("GLOBAL _climate_control");  
}

void climate_control(void)
{
    int t1, t2, t;
    while(1) {
        t1 = adc_read(0);
        t2 = adc_read(1);
        t = t1 - t2;
        if(t > 1) {
            heating_system(OFF);
            cooling_system(ON);
            stable_temperature(OFF);
        } else if (t < -1) {
            heating_system(ON);
            cooling_system(OFF);
            stable_temperature(OFF);
        } else {
            heating_system(OFF);
            cooling_system(OFF);
            stable_temperature(ON);
        }
    }
}

