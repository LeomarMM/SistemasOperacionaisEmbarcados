#include "user_tasks.h"
#include "semaphore.h"
#include "io.h"
#include "kernel.h"
#include <xc.h>
#include <stdio.h>
byte btab[4][3] =
{
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9},
    {10, 0, 11}
};

void config_tasks()
{
    asm("GLOBAL _climate_control, _elevator_control");  
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

void elevator_control(void)
{
    int column = 1, row = -1, button = -1;
    while(1)
    {
        int buf[3];
        enable_keypad_column(column);
        row = read_keypad_row();
        if(row)
        {
        di();
        sprintf(buf, "%i", btab[row-1][column-1]);
        lcd_set_cursor(1, 1);
        lcd_write_string(buf);
        ei();
        }
        column++;
    }
}
