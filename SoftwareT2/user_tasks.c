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
void system_boot(void*);
void fire_alarm_control(void*);
void temperature_control(void*);
void climate_control(void*);
void lcd_output(void*);
const TickType_t delay_ms = ((double)configTICK_RATE_HZ/1000.0);
const TickType_t delay_us = ((double)configTICK_RATE_HZ/1000000.0);
const char* temp_ctrl_str = "Temp. Cont.: --- \xDF""C";
const char* temp_amb_str = "Temp. Ambt.: --- \xDF""C";
const char* piso_slc_str = "Piso. Selec: ---";
const char* piso_atl_str = "Piso. Atual: ---";
int temp_ctrl = 25;
int temp_amb = 0;
void system_boot(void* ptr)
{
    lcd_init();
    lcd_set_cursor(1, 1);
    lcd_write_string(temp_ctrl_str);
    lcd_set_cursor(2, 1);
    lcd_write_string(temp_amb_str);
    lcd_set_cursor(3, 1);
    lcd_write_string(piso_slc_str);
    lcd_set_cursor(4, 1);
    lcd_write_string(piso_atl_str);
    xTaskCreate(temperature_control, "temperature_control", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(climate_control, "climate_control", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(fire_alarm_control, "fire_alarm_control", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(lcd_output, "lcd_output", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
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
            vTaskDelay(500*delay_ms);
            temp_ctrl++;      
        } else if (read_temperature_decrement_button() == 0) {
            vTaskDelay(500*delay_ms);
            temp_ctrl--;
        }
    }
}
void climate_control(void* ptr)
{
    int temp_amb_int, temp_ctrl_int;    
    while(1) {
        temp_amb_int = ((double)(adc_read())*(500.0/1024.0));
        temp_amb  = temp_amb_int;
        temp_ctrl_int = temp_ctrl;
        if(temp_amb_int > temp_ctrl_int) {
            heating_system(OFF);
            cooling_system(ON);
            stable_temperature(OFF);
        } else if (temp_amb_int < temp_ctrl_int) {
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
void lcd_output(void* ptr)
{
    int temp_ctrl_int, temp_amb_int;
    while(1) {
        char numbuf[4];
        
        temp_ctrl_int = temp_ctrl;
        temp_amb_int = temp_amb;
        
        sprintf(numbuf, "%3d", temp_ctrl_int);
        lcd_set_cursor(1, 14);
        lcd_write_string(numbuf);
        
        sprintf(numbuf, "%3d", temp_amb_int);
        lcd_set_cursor(2, 14);
        lcd_write_string(numbuf);
    }
}