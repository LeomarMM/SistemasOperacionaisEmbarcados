#ifndef IO_H
#define	IO_H

void hardware_init();

// PWM functions

// ADC functions
void adc_init(void);
int adc_read(int);
// LCD functions
void lcd_init(void);
void lcd_cursor(short x, short y);
void lcd_cmd(short cmd);
// 7SEG functions


#endif	/* IO_H */

