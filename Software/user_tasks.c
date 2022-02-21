#include "user_tasks.h"
#include "semaphore.h"
#include "io.h"
#include "kernel.h"
#include <xc.h>
#include <stdio.h>
const byte btab[4][3] =
{
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9},
    {10, 0, 11}
};
int current_floor = 0;
int next_floor = 0;
short moving = 0;
void config_tasks()
{
    asm("GLOBAL _climate_control, _elevator_control, _elevator_move");  
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
        di();
        __delay_ms(10);
        ei();
        row = read_keypad_row();
        di();
        if(row)
            if(btab[row-1][column-1] > 0 && btab[row-1][column-1] <= 9 && current_floor == next_floor) 
                next_floor = btab[row-1][column-1];
        ei();
        column++;
        if(column > 3) column = 1;
    }
}
void elevator_move(void)
{
    
    while(1)
    {
        if(next_floor != current_floor)
        {
            if(!moving)
            {
                di();
                moving = 1;
                if(next_floor < current_floor) elevator_down();
                if(next_floor > current_floor) elevator_up();
                ei();
            }
            delay_task(500);
            di();
            if(next_floor < current_floor) current_floor--;
            if(next_floor > current_floor) current_floor++;
            ei();
        }
        else 
        {
            di();
            if(moving) 
            {
                moving = 0;
                elevator_stop();
            }
            ei();
        }
    }
}