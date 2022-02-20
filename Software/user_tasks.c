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
  while (1) 
    LATDbits.LATD1 = ~PORTDbits.RD1;   
}

void adc_temp()
{
    semaphore_t sem;
    sem_create(&sem, 0);
    while (1) 
    {
        sem_wait(&sem);
        
        adc_read(0);
        
        sem_post(&sem);
    }
}
