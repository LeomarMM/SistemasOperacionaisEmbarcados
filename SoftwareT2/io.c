#include "io.h"
#include "FreeRTOS.h"
#include "task.h"
#define DELAY_MS ((double)configTICK_RATE_HZ/1000.0)
#define DELAY_uS ((double)configTICK_RATE_HZ/1000000.0)

void adc_init()
{
}
int adc_read(unsigned char port)
{   
    return 0;
}
void lcd_port(char a)
{
	if(a & 1)
    {
        asm("BSET LATD, #0");
    }
	else
		asm("BCLR LATD, #0");

	if(a & 2)
    {
        asm("BSET LATD, #1");
    }
	else
		asm("BCLR LATD, #1");

	if(a & 4)
    {
        asm("BSET LATD, #2");
    }
	else
		asm("BCLR LATD, #2");

	if(a & 8)
    {
        asm("BSET LATD, #3");
    }
	else
		asm("BCLR LATD, #3");
}
void lcd_cmd(char a)
{
	asm("BCLR LATD, #4");             // => RS = 0
	lcd_port(a);
	asm("BSET LATD, #5");             // => E = 1
    vTaskDelay(4 * DELAY_MS);
    asm("BCLR LATD, #5");             // => E = 0
}

void lcd_clear()
{
	lcd_cmd(0);
	lcd_cmd(1);
}

void lcd_set_cursor(char a, char b)
{
	char temp,z,y;
	if(a == 1) {
	  temp = 0x80 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		lcd_cmd(z);
		lcd_cmd(y);
	} else if(a == 2) {
		temp = 0xC0 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		lcd_cmd(z);
		lcd_cmd(y);
	} else if(a == 3) {
        temp = 0x94 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		lcd_cmd(z);
		lcd_cmd(y);
    } else if(a == 4) {
        temp = 0xD4 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		lcd_cmd(z);
		lcd_cmd(y);
    }
}

void lcd_init()
{
    lcd_port(0x00);
    vTaskDelay(20 * DELAY_MS);
    lcd_cmd(0x03);
    vTaskDelay(5 * DELAY_MS);
    lcd_cmd(0x03);
	vTaskDelay(16 * DELAY_MS);
    lcd_cmd(0x03);
    /////////////////////////////////////////////////////
    lcd_cmd(0x02);
    lcd_cmd(0x02);
    lcd_cmd(0x08);
    lcd_cmd(0x00);
    lcd_cmd(0x0C);
    lcd_cmd(0x00);
    lcd_cmd(0x06);
}

void lcd_write_char(char a)
{
    
   char temp,y;
   temp = a&0x0F;
   y = a&0xF0;
   asm("BSET LATD, #4");
   lcd_port(y>>4);             //Data transfer
   asm("BSET LATD, #5");
   vTaskDelay(40 * DELAY_uS);
   asm("BCLR LATD, #5");
   lcd_port(temp);
   asm("BSET LATD, #5");
   vTaskDelay(40 * DELAY_uS);
   asm("BCLR LATD, #5");
}

void lcd_write_string(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   lcd_write_char(a[i]);
}

void lcd_shift_right(void)
{
	lcd_cmd(0x01);
	lcd_cmd(0x0C);
}

void lcd_shift_left(void)
{
	lcd_cmd(0x01);
	lcd_cmd(0x08);
}

void heating_system(short state)
{
    if(state)
    {
        asm("BCLR PORTD, #13");
    }
    else asm("BSET PORTD, #13");
}

void cooling_system(short state)
{
    if(state)
    {
        asm("BCLR PORTD, #15");
    }
    else asm("BSET PORTD, #15");
}

void stable_temperature(short state)
{
    if(state)
    {
        asm("BCLR PORTD, #14");
    }
    else asm("BSET PORTD, #14");
}
void fire_alarm_buzzer(short state)
{
    if(state)
    {
        asm("BSET PORTF, #7");
    }
    else asm("BCLR PORTF, #7");
}

void fire_alarm_warning_lights(short state)
{
    if(state)
    {
        asm("BCLR PORTF, #8");
    }
    else asm("BSET PORTF, #8");
    asm("BSET PORTF, #12");
}

void fire_alarm_warning_lights_invert(void)
{
    
    PORTFbits.RF8 = ~PORTFbits.RF8;
    PORTFbits.RF12 = ~PORTFbits.RF12;
    
}

int read_fire_alarm_button(void)
{
    return PORTFbits.RF4;
}

int read_temperature_increment_button(void)
{
    return PORTFbits.RF5;
}

int read_temperature_decrement_button(void)
{
    return PORTFbits.RF6;
}

void enable_keypad_column(short column)
{   
    if(column == 1)
    {
        asm("BSET LATD, #8\n"
            "BCLR LATD, #7\n"
            "BCLR LATD, #6\n");
    }
    else if(column == 2)
    {
        asm("BCLR LATD, #8\n"
            "BSET LATD, #7\n"
            "BCLR LATD, #6\n");
    }
    else
    {
        asm("BCLR LATD, #8\n"
            "BCLR LATD, #7\n"
            "BSET LATD, #6\n");
    }  
}

int read_keypad_row(void)
{
    if(!PORTDbits.RD9)
        return 1;
    if(!PORTDbits.RD10)
        return 2;
    if(!PORTDbits.RD11)
        return 3;
    if(!PORTDbits.RD12)
        return 4;
    return 0;
}

void usart_init()
{
}

void usart_send_data(char data)
{    
}

char usart_read_data(void)
{
    return 'a';
}