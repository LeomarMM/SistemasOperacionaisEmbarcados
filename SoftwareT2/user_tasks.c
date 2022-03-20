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
    xTaskCreate(temperature_control, "temperature_control", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(climate_control, "climate_control", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(fire_alarm_control, "fire_alarm_control", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
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
    int t, tref;    
    while(1) {
        t = ((double)(adc_read())*(500.0/1024.0));
        tref = temperature;
        if(t > tref) {
            heating_system(OFF);
            cooling_system(ON);
            stable_temperature(OFF);
        } else if (t < tref) {
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