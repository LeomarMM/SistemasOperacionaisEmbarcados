/*
 * File:   taskdef.c
 * Author: Leomar
 *
 * Created on 18 de Março de 2022, 16:06
 */
#include "xc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "io.h"
void system_boot(void* ptr);
void fire_alarm_control(void* ptr);
const TickType_t delay_ms = ((double)configTICK_RATE_HZ/1000.0);
const TickType_t delay_us = ((double)configTICK_RATE_HZ/1000000.0);
void system_boot(void* ptr)
{
    hardware_init();
    lcd_write_string("Iniciando...");
    xTaskCreate(fire_alarm_control, "alarmctr", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    while(1);
}
void fire_alarm_control(void* ptr)
{
    while(1){
        if(read_fire_alarm_button() == 0) {
            fire_alarm_buzzer(ON);
            fire_alarm_warning_lights(ON);
            do {
                fire_alarm_warning_lights_invert();
                vTaskDelay(250 * delay_ms);
            } while (read_fire_alarm_button() == 0); 
            fire_alarm_buzzer(OFF);
            fire_alarm_warning_lights(OFF);
        } else {
            fire_alarm_buzzer(OFF);
            fire_alarm_warning_lights(OFF);
        }
        vTaskDelay(500 * delay_ms);
    }
}