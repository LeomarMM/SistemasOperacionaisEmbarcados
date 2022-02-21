#include "io.h"

void hardware_init(void)
{
    PORTA = 0x00;
    PORTB = 0b11111000;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    TRISA = 0b00000011; // RA0 e RA1 setados como entrada
    TRISB = 0b00000111; // RB0, RB1 e RB2 setados como entradas, demais pinos do portB como saída
    TRISC = 0b10000000;
    TRISD = 0b11110000;       // seta portD como saída.
    TRISE = 0b00000000;
    
    lcd_init();
}

void adc_init()
{
    ADCON1bits.VCFG = 0;
    ADCON1bits.PCFG = 0b1101;
    ADCON2bits.ADFM = 1;
    ADCON2bits.ACQT = 1;
    ADCON2bits.ADCS = 2;
    TRISAbits.RA0 = 1;
    TRISAbits.RA1 = 1;
    ADCON0bits.ADON = 1;
}

int adc_read(int port)
{
    ADCON0bits.CHS = port;
    ADCON0bits.GODONE = 1;
    while(ADCON0bits.GODONE);
    return ADRES;
}

void lcd_port(char a)
{
	if(a & 1)
		D4 = 1;
	else
		D4 = 0;

	if(a & 2)
		D5 = 1;
	else
		D5 = 0;

	if(a & 4)
		D6 = 1;
	else
		D6 = 0;

	if(a & 8)
		D7 = 1;
	else
		D7 = 0;
}
void lcd_cmd(char a)
{
	RS = 0;             // => RS = 0
	lcd_port(a);
	EN  = 1;             // => E = 1
    __delay_ms(4);
    EN  = 0;             // => E = 0
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
   __delay_ms(20);
  lcd_cmd(0x03);
	__delay_ms(5);
  lcd_cmd(0x03);
	__delay_ms(16);
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
   RS = 1;             // => RS = 1
   lcd_port(y>>4);             //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
   lcd_port(temp);
   EN = 1;
   __delay_us(40);
   EN = 0;
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
    PORTBbits.RB3 = (state == 1)? 0:1;
}

void cooling_system(short state)
{
    PORTBbits.RB5 = (state == 1)? 0:1;
}

void stable_temperature(short state)
{
    PORTBbits.RB4 = (state == 1)? 0:1;
}

void elevator_down(void)
{
    PORTEbits.RE0 = 0;
    PORTEbits.RE1 = 1;
}

void elevator_up(void)
{
    PORTEbits.RE1 = 0;
    PORTEbits.RE0 = 1;
}

void elevator_stop(void)
{
    PORTEbits.RE0 = 0;
    PORTEbits.RE1 = 0;
}

void fire_alarm_buzzer(short state)
{
    PORTDbits.RD0 = (state == 1)? 1:0;
}

void fire_alarm_warning_lights(short state)
{
    PORTBbits.RB6 = (state == 1)? 1:0;
    PORTBbits.RB7 = 0;
}

void fire_alarm_warning_lights_invert(void)
{
    PORTBbits.RB6 = ~PORTBbits.RB6;
    PORTBbits.RB7 = ~PORTBbits.RB7;
}