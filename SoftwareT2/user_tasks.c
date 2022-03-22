#include "xc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "io.h"
#include "user_tasks.h"
#include "semphr.h"
const TickType_t delay_s = configTICK_RATE_HZ;
const TickType_t delay_ms = ((double)configTICK_RATE_HZ/1000.0);
const TickType_t delay_us = ((double)configTICK_RATE_HZ/1000000.0);
const uint8_t btab[4][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 0, 11}};
const double fat_corr = 500.0/1023.0;
const char* temp_ctrl_str = "Temp. Cont.: --- \xDF""C";
const char* temp_amb_str = "Temp. Ambt.: --- \xDF""C";
const char* piso_slc_str = "Piso. Selec: ---";
const char* piso_atl_str = "Piso. Atual: ---";

SemaphoreHandle_t cfh;
SemaphoreHandle_t nfh;
SemaphoreHandle_t tch;
SemaphoreHandle_t tah;

uint8_t current_floor = 1;
uint8_t next_floor = 1;
int16_t temp_ctrl = 25;
int16_t temp_amb = 0;

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
    
    cfh = xSemaphoreCreateMutex();
    nfh = xSemaphoreCreateMutex();
    tch = xSemaphoreCreateMutex();
    tah = xSemaphoreCreateMutex();
    
    xTaskCreate(temperature_control, "temperature_control", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(climate_control, "climate_control", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(fire_alarm_control, "fire_alarm_control", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES, NULL);
    xTaskCreate(lcd_output, "lcd_output", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
    xTaskCreate(elevator_control, "elevator_control", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(elevator_move, "elevator_move", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
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
            xSemaphoreTake(tch, portMAX_DELAY);
            temp_ctrl++;    
            xSemaphoreGive(tch);
        } else if (read_temperature_decrement_button() == 0) {
            vTaskDelay(500*delay_ms);
            xSemaphoreTake(tch, portMAX_DELAY);
            temp_ctrl--;
            xSemaphoreGive(tch);
        }
    }
}
void climate_control(void* ptr)
{
    int16_t temp_amb_int, temp_ctrl_int;    
    while(1) {
        temp_amb_int = adc_read()*fat_corr;
        xSemaphoreTake(tah, portMAX_DELAY);
        temp_amb  = temp_amb_int;
        xSemaphoreGive(tah);
        xSemaphoreTake(tch, portMAX_DELAY);
        temp_ctrl_int = temp_ctrl;
        xSemaphoreGive(tch);
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
    int16_t temp_ctrl_int, temp_amb_int;
    uint8_t current_floor_int, next_floor_int;
    char numbuf[4];
    while(1) {
        
        xSemaphoreTake(tch, portMAX_DELAY);
        temp_ctrl_int = temp_ctrl;
        xSemaphoreGive(tch);
        
        xSemaphoreTake(tah, portMAX_DELAY);
        temp_amb_int = temp_amb;
        xSemaphoreGive(tah);
        
        xSemaphoreTake(cfh, portMAX_DELAY);
        current_floor_int = current_floor;
        xSemaphoreGive(cfh);
        
        xSemaphoreTake(nfh, portMAX_DELAY);
        next_floor_int = next_floor;
        xSemaphoreGive(nfh);
        
        sprintf(numbuf, "%3d", temp_ctrl_int);
        lcd_set_cursor(1, 14);
        lcd_write_string(numbuf);
        
        sprintf(numbuf, "%3d", temp_amb_int);
        lcd_set_cursor(2, 14);
        lcd_write_string(numbuf);
        
        sprintf(numbuf, "%3d", next_floor_int);
        lcd_set_cursor(3, 14);
        lcd_write_string(numbuf);
        
        sprintf(numbuf, "%3d", current_floor_int);
        lcd_set_cursor(4, 14);
        lcd_write_string(numbuf);        
    }
}

void elevator_control(void* ptr)
{
    int column = 1, row = 0;
    uint8_t current_floor_int;
    while(1)
    {
        enable_keypad_column(column);
        vTaskDelay(10*delay_ms);
        row = read_keypad_row();
        xSemaphoreTake(cfh, portMAX_DELAY);
        current_floor_int = current_floor;
        xSemaphoreGive(cfh);
        if(row) if(btab[row-1][column-1] > 0 && btab[row-1][column-1] <= 9 && current_floor_int == next_floor)
        {
            xSemaphoreTake(nfh, portMAX_DELAY);
            next_floor = btab[row-1][column-1];
            xSemaphoreGive(nfh);
        }
        column++;
        if(column > 3) column = 1;
    }
}
void elevator_move(void* ptr)
{
    uint8_t moving = 0;
    uint8_t next_floor_int = 1;
    while(1)
    {
        xSemaphoreTake(nfh, portMAX_DELAY);
        next_floor_int = next_floor;
        xSemaphoreGive(nfh);
        if(next_floor_int != current_floor)
        {
            if(!moving)
            {
                moving = 1;
                if(next_floor_int < current_floor) elevator_down();
                if(next_floor_int > current_floor) elevator_up();
            }
            vTaskDelay(delay_s);
            xSemaphoreTake(cfh, portMAX_DELAY);
            if(next_floor_int < current_floor) current_floor--;
            if(next_floor_int > current_floor) current_floor++;
            xSemaphoreGive(cfh);
        }
        else 
        {
            if(moving) 
            {
                moving = 0;
                elevator_stop();
            }
        }
    }
}
