#include "user_tasks.h"
#include "semaphore.h"
#include "io.h"
#include "kernel.h"
#include <xc.h>
void config_tasks()
{
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD2 = 0;
    TRISBbits.RB3 = 0;
    TRISBbits.RB4 = 0;
    TRISBbits.RB5 = 0;
    asm("GLOBAL _pisca_led_1, _pisca_led_2, _adc_temp");  
}

void pisca_led_1()
{  
    while (1)
    {
        LATDbits.LATD0 = ~PORTDbits.RD0;
        delay_task(1000);
    }
}

void pisca_led_2()
{
    while(1)
        LATDbits.LATD1 = ~PORTDbits.RD1;   
}

void adc_temp()
{
    #define margem 1
    semaphore_t sem;
    sem_create(&sem, 1);
    int V2, V1, T;
    while (1) 
    {
        //sem_wait(&sem);
        V2 = adc_read(0);
        V1 = adc_read(1);
        //sem_post(&sem);
        T = (V2-V1)/2;
        LATBbits.LATB3 = (T > (50-margem));
        LATBbits.LATB4 = (T >= (50+margem) && T <= (50-margem));
        LATBbits.LATB5 = (T < (50+margem));
    }
}
