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
void temperature_control(void* ptr);
void climate_control(void* ptr);
const TickType_t delay_ms = ((double)configTICK_RATE_HZ/1000.0);
const TickType_t delay_us = ((double)configTICK_RATE_HZ/1000000.0);
int temperature = 25;
void system_boot(void* ptr)
{
    hardware_init();
    xTaskCreate(temperature_control, "tempctr", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(climate_control, "climctr", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(fire_alarm_control, "alarmctr", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    vTaskDelete(0);
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
void temperature_control(void* ptr)
{
    while (1) {
        if(read_temperature_increment_button() == 0) {
            vTaskDelay(delay_ms);
            temperature++;      
        } else if (read_temperature_decrement_button() == 0) {
            vTaskDelay(delay_ms);
            temperature--;
        }
    }
}
void climate_control(void* ptr)
{
    int t1, t2, t;    
    while(1) {
        char buffer[50];
        t1 = adc_read(1);
        t2 = adc_read(0);
        t = ((double)(t1-t2)*(500.0/1024.0));
        lcd_set_cursor(2, 1);
        sprintf(buffer, "Temp. Ambt.: %d C      ", (t));
        lcd_write_string(buffer);
        int tref = temperature;
        if(t > (tref)) {
            heating_system(OFF);
            cooling_system(ON);
            stable_temperature(OFF);
        } else if (t < (tref)) {
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